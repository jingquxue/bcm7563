#ifndef KWT_OUTPUT_H
#define KWT_OUTPUT_H

eRet InitializeComposite(void);
NEXUS_VideoOutput CompositeGetConnectorV(void);
eRet InitializeHdmi(void);
void SetWidgetEngine(void);
NEXUS_VideoOutput GetHdmiConnectorV ();
void UninitializeHdmi(void);
eRet InitializeAudioDac(void);
eRet ConnectAudioDacOutput (NEXUS_AudioInput input);
#endif

