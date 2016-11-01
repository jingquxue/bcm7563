#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <ctype.h>

#include "main.h"
#include "transmitter.h"
#include "menu_tcpipconfiguration.h"
#include "global.h"

pthread_t gTransmitterThreadId;
IpStreamerCtx *ipStreamerCtx = NULL;
B_PlaybackIpLiveStreamingHandle liveStreamingHandle = NULL;

unsigned char TransmitterReboot = 0;

static void dataReadyCallbackIpDst (void *context, int param)
{
	IpStreamerCtx *ipStreamerCtx = (IpStreamerCtx *) context;

	BSTD_UNUSED (param);

	if (!ipStreamerCtx || !ipStreamerCtx->ipDst)
		return;

	BKNI_SetEvent (ipStreamerCtx->ipDst->dataReadyEvent);
}

int buildRtpHeader (unsigned short nextSeq, RtpHeader *rh, int payloadType, bool lastPktInAvFrame)
{
	memset (rh, 0, sizeof (RtpHeader));
	rh->b.v = 2;
	if (lastPktInAvFrame)
		rh->b.m = 1;
	rh->b.pt = payloadType;
	rh->timestamp = 0;
	rh->csrc = 0x12121212;
	rh->b.seq = htons (nextSeq);
	return (sizeof (RtpHeader));
}

int buildRtpIovec (struct bfile_io_write_net *data, unsigned char *buf, int bufSize,
																	int *totalBytesInIovec, int *totalPayloadCopied)
{
	int offset, i, bytesToSend, rtpHdrSize;
	bool lastPktInAvFrame = false;
	struct bfile_io_write_net *file = (struct bfile_io_write_net *) data;
	unsigned char rtp_header[32];
	unsigned char *rtpHdr = rtp_header;

	for (offset = 0, i = 0, *totalBytesInIovec = 0, *totalPayloadCopied = 0;
					bufSize > 0 && i < B_RTP_IOVEC;
					i += 2, offset += RTP_UDP_AV_PAYLOAD_SIZE, bufSize -= bytesToSend)
	{
		if (bufSize <= RTP_UDP_AV_PAYLOAD_SIZE)
		{
			lastPktInAvFrame = true;
		}
		else
		{
			lastPktInAvFrame = false;
		}
		rtpHdrSize = buildRtpHeader (data->nextSeq++, (RtpHeader *) rtpHdr, data->rtpPayloadType, lastPktInAvFrame);

		bytesToSend = bufSize >= RTP_UDP_AV_PAYLOAD_SIZE ? RTP_UDP_AV_PAYLOAD_SIZE : bufSize;
		file->iovec[i].iov_base = (char *) rtpHdr;
		file->iovec[i].iov_len = rtpHdrSize;
		file->iovec[i+1].iov_base = buf + offset;
		file->iovec[i+1].iov_len = bytesToSend;
		*totalBytesInIovec += bytesToSend + rtpHdrSize;
		*totalPayloadCopied += bytesToSend;
	}

	return i;
}

int buildUdpIovec (struct bfile_io_write_net *data, unsigned char *buf,
												int bufSize, int *totalBytesInIovec, int *totalPayloadCopied)
{
	int offset, i, bytesToSend;
	struct bfile_io_write_net *file = (struct bfile_io_write_net *) data;

	for (offset = 0, i = 0, *totalBytesInIovec = 0, *totalPayloadCopied = 0;
								bufSize > 0 && i < B_UDP_IOVEC;
								i += 1, offset += RTP_UDP_AV_PAYLOAD_SIZE, bufSize -= bytesToSend)
	{
		bytesToSend = bufSize >= RTP_UDP_AV_PAYLOAD_SIZE ? RTP_UDP_AV_PAYLOAD_SIZE : bufSize;
		file->iovec[i].iov_base = buf + offset;
		file->iovec[i].iov_len = bytesToSend;
		*totalBytesInIovec += bytesToSend;
		*totalPayloadCopied += bytesToSend;
	}
	return i;
}

int sendRtpUdpChunk (struct bfile_io_write_net *data, unsigned char *buf, int bufSize)
{
	int bytesWritten = 0, totalWritten = 0, iovecCount;
	struct msghdr msg;
	int totalBytesToWrite;
	int totalPayloadCopied;

	data->streamingSockAddr.sin_addr.s_addr = htonl (g_Config.NetRelaySettings.RemoteIP);
	data->streamingSockAddr.sin_port = htons (g_Config.NetRelaySettings.RemotePort);

	memset (&msg, 0, sizeof (struct msghdr));
	msg.msg_name = (struct sockaddr *) &data->streamingSockAddr;
	msg.msg_namelen = sizeof (struct sockaddr_in);

	while (bufSize > 0)
	{
		if (g_Config.NetRelaySettings.protocal == IpProtocol_eUdp)
		{
			data->streamingProtocol = B_PlaybackIpProtocol_eUdp;
		}
		else
		{
			data->streamingProtocol = B_PlaybackIpProtocol_eRtp;
		}

		if (data->streamingProtocol == B_PlaybackIpProtocol_eRtp)
		{
			iovecCount = buildRtpIovec (data, buf, bufSize, &totalBytesToWrite, &totalPayloadCopied);
		}
		else
		{
			iovecCount = buildUdpIovec (data, buf, bufSize, &totalBytesToWrite, &totalPayloadCopied);
		}
		msg.msg_iov = data->iovec;
		msg.msg_iovlen = iovecCount;


		if ((bytesWritten = sendmsg (data->fd, &msg, 0)) <= 0)
		{
			perror ("ERROR sendmsg():");
			goto out;
		}

		if (totalBytesToWrite != bytesWritten)
		{
			printf ("%s: Failed to sendmsg all bytes: asked %d, sent %d", __FUNCTION__, totalBytesToWrite, bytesWritten);
			goto out;
		}

		buf += totalPayloadCopied;
		bufSize -= totalPayloadCopied;
		totalWritten += totalPayloadCopied;
	}

out:
	return totalWritten;
}

ssize_t B_PlaybackIp_UtilsStreamingCtxWrite (bfile_io_write_t self, const void *buf, size_t length)
{
	struct bfile_io_write_net *file = (struct bfile_io_write_net *) self;
	ssize_t rc;
	size_t bytesSent = 0;
	int writeQueueDepth;
	unsigned int bytesConsumed = 0;

	size_t writeQueueSpaceAvail;
	int outBufLength;
	void *outBuf;

	BDBG_ASSERT(file);

	if (ioctl (file->fd, SIOCOUTQ, &writeQueueDepth))
	{
		printf ("%s: failed to get tcp write q depth for socket %d", __FUNCTION__, file->fd);
		writeQueueDepth = 0;
	}

	writeQueueSpaceAvail = file->writeQueueSize - writeQueueDepth;

	if (length > writeQueueSpaceAvail)
	{
		printf ("Write trimming # of bytes to write (%d) to the write q available space (%d) for socket %d\n",
																				length, writeQueueSpaceAvail, file->fd);
		length = writeQueueSpaceAvail;
	}

	if (length > 0)
	{
		outBuf = (void *) buf;
		outBufLength = length;

		rc = sendRtpUdpChunk (file, (unsigned char *) outBuf, outBufLength);
		if (rc != outBufLength)
		{
			printf ("%s: Failed to send %d bytes, sent %d for socket %d", __FUNCTION__, outBufLength, rc, file->fd);
			return -1;
		}
		bytesSent += rc;
		bytesConsumed += rc;
	}

	file->totalBytesConsumed += bytesConsumed;

	return bytesConsumed;
}

ssize_t B_PlaybackIp_UtilsStreamingCtxWriteAll (bfile_io_write_t self, const void *buf, size_t bufSize)
{
	int bytesWritten = 0;
	ssize_t totalWritten = 0;
	int bytesToWrite;

	struct bfile_io_write_net *file = (struct bfile_io_write_net *) self;
	if (file->fd <= 0)
	{
		printf ("%s: streaming session is not yet enabled, skipping %d bytes", __FUNCTION__, bufSize);
		return bufSize;
	}
	while (bufSize)
	{
		if (file->stopStreaming == true)
		{
			printf ("%s: app wants to stop streaming, breaking out for fd %d", __FUNCTION__, file->fd);
			return -1;
		}
		bytesToWrite = bufSize;
		bytesWritten = B_PlaybackIp_UtilsStreamingCtxWrite ((struct bfile_io_write *) file,
																				(void *) buf, (size_t) bytesToWrite);
		if (bytesWritten < 0)
		{
			return bytesWritten;
		}
		else if (bytesWritten == 0)
		{
			printf ("%s: Select timeout, # of timeouts %d", __FUNCTION__, file->selectTimeouts);
			file->selectTimeouts++;

			if (file->liveStreaming && file->selectTimeouts > MAX_SELECT_TIMEOUT_COUNT)
			{
				printf ("client is not receiving data, return fatal error\n");
				return -1;
			}
			continue;
		}
		file->selectTimeouts = 0;
		buf = (unsigned char *) buf + bytesWritten;
		bufSize -= bytesWritten;
		totalWritten += bytesWritten;
		if (bufSize)
		{
			printf ("%s: wrote %d bytes, to write %d\n", __FUNCTION__, bytesWritten, bufSize);
		}
	}

	return totalWritten;
}

static void liveStreamingThreadFromRaveBuffer (void *data)
{
	int rc;
	BSTD_UNUSED(data);
	void *readBuf = NULL;
	size_t clearBufferSize = TS_PKT_SIZE * HTTP_AES_BLOCK_SIZE * STREAMING_BUF_MULTIPLE * 4;
	size_t bytesRead = 0, bytesToRead, totalBytesRead;
	int bytesWritten = 0, bytesToWrite;
	int readTimeouts = 0;
	bool gotErrorInStreamingLoop = false;

	if (g_Config.NetRelaySettings.Enable == 0)
	{
		ipStreamerCtx->ipDst->liveStreamingHandle->stop = true;
	}
	else
	{
		ipStreamerCtx->ipDst->liveStreamingHandle->stop = false;
	}

	while (!ipStreamerCtx->ipDst->liveStreamingHandle->stop)
	{
		rc = BKNI_WaitForEvent(ipStreamerCtx->ipDst->liveStreamingHandle->dataReadyEvent, 260);
		if (rc != 0 && rc != BERR_TIMEOUT)
		{
			printf ("%s: dataReadyEvent ERROR (%d), breaking out of streaming loop ", __FUNCTION__, rc);
			break;
		}
		BKNI_ResetEvent(ipStreamerCtx->ipDst->liveStreamingHandle->dataReadyEvent);

		if (readTimeouts * 260 > 5000)
		{
			printf ("Error: readTimeouts=%d\n", readTimeouts);

			TransmitterReboot = 1;
			break;
		}
		bytesToRead = clearBufferSize;
		totalBytesRead = 0;

		while (totalBytesRead < bytesToRead)
		{
			if (NEXUS_Recpump_GetDataBuffer (ipStreamerCtx->ipDst->liveStreamingHandle->recpumpHandle,
														(const void **) &readBuf, (size_t *) &bytesRead) != NEXUS_SUCCESS)
			{
				printf ("%s: NEXUS_Recpump_GetDataBuffer failed, breaking out of streaming loop", __FUNCTION__);
				gotErrorInStreamingLoop = true;
				break;
			}

			if (readBuf == NULL || bytesRead <= 0)
			{
				readTimeouts++;
				break;
			}
			readTimeouts = 0;
			if (bytesRead > (bytesToRead - totalBytesRead))
				bytesRead = bytesToRead - totalBytesRead;
			if (bytesRead > clearBufferSize)
				bytesRead = clearBufferSize;
			bytesToRead = bytesRead;
			if (bytesToRead <= 0)
			{
				printf ("%s: recpump underflow (bytesRead %d), sleeping for 100msec...", __FUNCTION__, bytesRead);
				continue;
			}
			bytesToWrite = bytesRead;
			bytesWritten = B_PlaybackIp_UtilsStreamingCtxWriteAll ((struct bfile_io_write *) &ipStreamerCtx->ipDst->liveStreamingHandle->data,
																									readBuf, bytesToWrite);
			if (bytesWritten <= 0)
			{
				gotErrorInStreamingLoop = true;
				if (NEXUS_Recpump_DataReadComplete (ipStreamerCtx->ipDst->liveStreamingHandle->recpumpHandle, 0) != NEXUS_SUCCESS)
				{
					printf ("%s: NEXUS_Recpump_DataReadComplete failed, breaking out of streaming loop", __FUNCTION__);
					break;
				}
				break;
			}
			if (NEXUS_Recpump_DataReadComplete (ipStreamerCtx->ipDst->liveStreamingHandle->recpumpHandle, bytesWritten) != NEXUS_SUCCESS)
			{
				printf ("%s: NEXUS_Recpump_DataReadComplete failed, breaking out of streaming loop", __FUNCTION__);
				gotErrorInStreamingLoop = true;
				break;
			}
			totalBytesRead += bytesRead;
		}

		if (readTimeouts)
		{
			printf ("wait until more data is received readTimeouts=%d\n", readTimeouts);
			continue;
		}
		if (gotErrorInStreamingLoop == true)
		{
			printf ("%s: gotErrorInStreamingLoop: breaking out of streaming loop", __FUNCTION__);
			break;
		}
	}

	return;
}


void StartTransmitter ()
{
	ipStreamerCtx->ipDst->liveStreamingHandle->streamingThread = B_Thread_Create ("Live Streamer",
													(B_ThreadFunc) liveStreamingThreadFromRaveBuffer, (void *) NULL, NULL);
	if (ipStreamerCtx->ipDst->liveStreamingHandle->streamingThread == NULL)
	{
		printf ("%s: Failed to create HTTP media file streaming thread \n", __FUNCTION__);
		return;
	}
}

void TransmitterInit ()
{
	NEXUS_Error rc = NEXUS_UNKNOWN;
	socklen_t len;
	struct ifreq ifr;
	struct sockaddr_in sa_loc;
	NEXUS_RecpumpOpenSettings recpumpOpenSettings;
	NEXUS_RecpumpSettings recpumpSettings;
	NEXUS_RecpumpAddPidChannelSettings pidSettings;

	if ((ipStreamerCtx = (IpStreamerCtx *) BKNI_Malloc (sizeof (IpStreamerCtx))) == NULL)
	{
		printf ("[%s]%d *** BKNI_Malloc Failure ***\n", __FUNCTION__, __LINE__);
		return;
	}
	memset (ipStreamerCtx, 0, sizeof (IpStreamerCtx));

	if (BKNI_CreateEvent (&(&ipStreamerCtx->ipDstList)->dataReadyEvent))
	{
		printf ("BKNI_CreateMutex failed");
		return;
	}

	NEXUS_Recpump_GetDefaultOpenSettings (&recpumpOpenSettings);
	recpumpOpenSettings.data.dataReadyThreshold = recpumpOpenSettings.data.atomSize * 22;
	(&ipStreamerCtx->ipDstList)->recpumpHandle = NEXUS_Recpump_Open (NEXUS_ANY_ID, &recpumpOpenSettings);
	if (!(&ipStreamerCtx->ipDstList)->recpumpHandle)
	{
		printf ("NEXUS Error at %d, returning..", __LINE__);
		return;
	}
	NEXUS_Recpump_GetSettings ((&ipStreamerCtx->ipDstList)->recpumpHandle, &recpumpSettings);

	recpumpSettings.data.dataReady.callback = dataReadyCallbackIpDst;
	recpumpSettings.data.dataReady.context = ipStreamerCtx;

	if (NEXUS_Recpump_SetSettings ((&ipStreamerCtx->ipDstList)->recpumpHandle, &recpumpSettings) != NEXUS_SUCCESS)
	{
		printf ("%s: Failed to update recpump settings", __FUNCTION__);
		return;
	}

	liveStreamingHandle = (B_PlaybackIpLiveStreaming *) BKNI_Malloc (sizeof (B_PlaybackIpLiveStreaming));
	if (!liveStreamingHandle)
	{
		printf ("%s: memory allocation failure\n", __FUNCTION__);
	}
	memset (liveStreamingHandle, 0, sizeof (B_PlaybackIpLiveStreaming));

	liveStreamingHandle->recpumpHandle = (&ipStreamerCtx->ipDstList)->recpumpHandle;
	liveStreamingHandle->dataReadyEvent = (&ipStreamerCtx->ipDstList)->dataReadyEvent;

	(&ipStreamerCtx->ipDstList)->liveStreamingHandle = liveStreamingHandle;

	NEXUS_Recpump_GetDefaultAddPidChannelSettings (&pidSettings);
	pidSettings.pidType = NEXUS_PidType_eVideo;
	pidSettings.pidTypeSettings.video.index = false;
	pidSettings.pidTypeSettings.video.codec = NEXUS_VideoCodec_eH264;
	pidSettings.pidTypeSettings.video.pid = g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].VidPid;

	ipStreamerCtx->videoPidChannel = NEXUS_PidChannel_Open (g_System.ParserBand,
					g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].VidPid, NULL);
	rc = NEXUS_Recpump_AddPidChannel ((&ipStreamerCtx->ipDstList)->recpumpHandle, ipStreamerCtx->videoPidChannel, &pidSettings);
	if (rc != NEXUS_SUCCESS)
	{
		return;
	}

	ipStreamerCtx->audioPidChannel = NEXUS_PidChannel_Open (g_System.ParserBand,
					g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].AudPid, NULL);
	rc = NEXUS_Recpump_AddPidChannel ((&ipStreamerCtx->ipDstList)->recpumpHandle, ipStreamerCtx->audioPidChannel, NULL);
	if (rc != NEXUS_SUCCESS)
	{
		return;
	}

	ipStreamerCtx->pcrPidChannel = NEXUS_PidChannel_Open (g_System.ParserBand,
					g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].PcrPid, NULL);
	rc = NEXUS_Recpump_AddPidChannel ((&ipStreamerCtx->ipDstList)->recpumpHandle, ipStreamerCtx->pcrPidChannel, NULL);
	if (rc != NEXUS_SUCCESS)
	{
		return;
	}

	ipStreamerCtx->patPidChannel = NEXUS_PidChannel_Open (g_System.ParserBand, 0x0000, NULL);
	rc = NEXUS_Recpump_AddPidChannel ((&ipStreamerCtx->ipDstList)->recpumpHandle, ipStreamerCtx->patPidChannel, NULL);
	if (rc != NEXUS_SUCCESS)
	{
		return;
	}

	ipStreamerCtx->pmtPidChannel = NEXUS_PidChannel_Open (g_System.ParserBand,
					g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].PmtPid, NULL);
	rc = NEXUS_Recpump_AddPidChannel ((&ipStreamerCtx->ipDstList)->recpumpHandle, ipStreamerCtx->pmtPidChannel, NULL);
	if (rc != NEXUS_SUCCESS)
	{
		return;
	}
	ipStreamerCtx->ipDst = &ipStreamerCtx->ipDstList;

	rc = NEXUS_Recpump_Start (ipStreamerCtx->ipDst->recpumpHandle);
	if (rc)
	{
		printf ("NEXUS Error at %d, returning..", __LINE__);
		return;
	}
	ipStreamerCtx->ipDst->liveStreamingHandle->data.minTime.tv_usec = 2 * 1000 * 1000 * 1000;

	if ((ipStreamerCtx->ipDst->liveStreamingHandle->data.fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror ("socket:");
	}

	memset (&ifr, 0, sizeof (ifr));
	memset (&sa_loc, 0, sizeof (sa_loc));
	strncpy (ifr.ifr_name, "eth0", sizeof (ifr.ifr_name) - 1);
	memcpy (&ifr.ifr_ifru.ifru_dstaddr, &sa_loc, sizeof (sa_loc));

	if (setsockopt (ipStreamerCtx->ipDst->liveStreamingHandle->data.fd, SOL_SOCKET, SO_BINDTODEVICE, (void *) &ifr, sizeof (ifr)) < 0)
	{
		perror ("SO_BINDTODEVICE");
	}

	len = sizeof (ipStreamerCtx->ipDst->liveStreamingHandle->data.writeQueueSize);

	if (getsockopt (ipStreamerCtx->ipDst->liveStreamingHandle->data.fd, SOL_SOCKET, SO_SNDBUF, &ipStreamerCtx->ipDst->liveStreamingHandle->data.writeQueueSize, &len))
	{
		printf ("ERROR: can't get socket write q size\n");
	}

	if ((ipStreamerCtx->ipDst->liveStreamingHandle->data.iovec = (struct iovec *) BKNI_Malloc(sizeof (struct iovec) * B_RTP_IOVEC)) == NULL)
	{
		printf ("%s: memory allocation failure at %d\n", __FUNCTION__, __LINE__);
	}

	ipStreamerCtx->ipDst->liveStreamingHandle->data.rtpPayloadType = 33;
}

void FreeMemTransmitter ()
{
	if (ipStreamerCtx->ipDst->liveStreamingHandle->data.iovec)
	{
		BKNI_Free(ipStreamerCtx->ipDst->liveStreamingHandle->data.iovec);
	}

	if (ipStreamerCtx)
	{
		BKNI_Free(ipStreamerCtx);
	}

	if (liveStreamingHandle)
	{
		BKNI_Free(liveStreamingHandle);
	}
}

void TransRebootThread (void *pParam)
{
	BSTD_UNUSED(pParam);
	TransmitterReboot = 0;
	while (1)
	{
		if (TransmitterReboot == 1)
		{
			printf ("----TransmitterReboot----\n");
			TransmitterReboot = 0;
			sleep (1);
			StartTransmitter ();
		}
		sleep (1);
	}
}

