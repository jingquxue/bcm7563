#ifndef KWT_TRANSMITTERS_H
#define KWT_TRANSMITTERS_H

#include <linux/sockios.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "nexus_pid_channel.h"
#include "nexus_parser_band.h"
#include "nexus_record.h"
#include "nexus_playpump.h"
#include "b_os_lib.h"
#include "nexus_core_utils.h"
#include "bfile_stdio.h"
#include "ts_psi.h"

#define TS_PKT_SIZE 188
#define HTTP_AES_BLOCK_SIZE 16
#define STREAMING_BUF_MULTIPLE 32
#define MAX_SELECT_TIMEOUT_COUNT 50
#define B_RTP_IOVEC 2
#define B_UDP_IOVEC 1
#define RTP_UDP_AV_PAYLOAD_SIZE 1316

typedef struct B_PlaybackIpLiveStreaming *B_PlaybackIpLiveStreamingHandle;

typedef struct RtpHeaderBits
{
	unsigned int cc:4;
	unsigned int x:1;
	unsigned int p:1;
	unsigned int v:2;
	unsigned int pt:7;
	unsigned int m:1;
	unsigned int seq:16;
} RtpHeaderBits;

typedef struct RtpHeader
{
	RtpHeaderBits b;
	int timestamp;
	int csrc;
} RtpHeader;

typedef enum B_PlaybackIpProtocol
{
	B_PlaybackIpProtocol_eUdp,
	B_PlaybackIpProtocol_eRtp
} B_PlaybackIpProtocol;

struct bfile_io_write_net
{
	struct bfile_io_write self;
	int fd;
	int writeQueueSize;
	int selectTimeouts;
	bool liveStreaming;
	B_PlaybackIpProtocol streamingProtocol;
	struct sockaddr_in streamingSockAddr;
	int rtpPayloadType;
	unsigned short nextSeq;
	struct iovec *iovec;
	uint64_t totalBytesConsumed;
	struct timeval minTime;
	bool stopStreaming;
};

typedef struct B_PlaybackIpLiveStreaming
{
	NEXUS_RecpumpHandle recpumpHandle;
	int fd;
	struct bfile_io_write_net data;
	B_ThreadHandle streamingThread;
	bool stop;
	BKNI_EventHandle dataReadyEvent;
} B_PlaybackIpLiveStreaming;

typedef struct IpDst
{
	NEXUS_RecpumpHandle recpumpHandle;
	B_PlaybackIpLiveStreamingHandle liveStreamingHandle;
	BKNI_EventHandle dataReadyEvent;
} IpDst;

typedef struct IpStreamerCtx
{
	NEXUS_PidChannelHandle videoPidChannel;
	NEXUS_PidChannelHandle audioPidChannel;
	NEXUS_PidChannelHandle pcrPidChannel;
	NEXUS_PidChannelHandle patPidChannel;
	NEXUS_PidChannelHandle pmtPidChannel;
	IpDst *ipDst;
	IpDst ipDstList;
}IpStreamerCtx;

extern IpStreamerCtx *ipStreamerCtx;

void TransRebootThread (void *pParam);

void TransmitterInit ();

void StartTransmitter ();

void FreeMemTransmitter ();

#endif

