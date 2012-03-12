#include "../../common.h"

/**
 * Adhoc Emulator PTP Passive Socket Creator
 * @param saddr Local MAC (Unused)
 * @param sport Local Binding Port
 * @param bufsize Socket Buffer Size
 * @param rexmt_int Retransmit Interval (in Microseconds)
 * @param rexmt_cnt Retransmit Count
 * @param backlog Size of Connection Queue
 * @param flag Bitflags (Unused)
 * @return Socket ID >= 0 on success or... ADHOC_NOT_INITIALIZED, ADHOC_INVALID_ARG, ADHOC_INVALID_ADDR, ADHOC_INVALID_PORT, ADHOC_SOCKET_ID_NOT_AVAIL, ADHOC_PORT_NOT_AVAIL, ADHOC_PORT_IN_USE, NET_NO_SPACE
 */
int proNetAdhocPtpListen(const SceNetEtherAddr * saddr, uint16_t sport, uint32_t bufsize, uint32_t rexmt_int, int rexmt_cnt, int backlog, int flag)
{
	// Library is initialized
	if(_init)
	{
		// Valid Address
		if(saddr != NULL && _IsLocalMAC(saddr))
		{
			// Random Port required
			if(sport == 0)
			{
				// Find unused Port
				while(sport == 0 || _IsPTPPortInUse(sport))
				{
					// Generate Port Number
					sport = (uint16_t)_getRandomNumber(65535);
				}
			}
			
			// Valid Ports
			if(!_IsPTPPortInUse(sport))
			{
				// Valid Arguments
				if(bufsize > 0 && rexmt_int > 0 && rexmt_cnt > 0 && backlog > 0)
				{
					// Create Infrastructure Socket
					int socket = sceNetInetSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					
					// Valid Socket produced
					if(socket > 0)
					{
						// Enable Port Re-use
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &_one, sizeof(_one));
						sceNetInetSetsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &_one, sizeof(_one));
						
						// Binding Information for local Port
						SceNetInetSockaddrIn addr;
						addr.sin_len = sizeof(addr);
						addr.sin_family = AF_INET;
						addr.sin_addr = INADDR_ANY;
						addr.sin_port = sceNetHtons(sport);
						
						// Bound Socket to local Port
						if(sceNetInetBind(socket, (SceNetInetSockaddr *)&addr, sizeof(addr)) == 0)
						{
							// Switch into Listening Mode
							if(sceNetInetListen(socket, backlog) == 0)
							{
								// Allocate Memory
								SceNetAdhocPtpStat * internal = (SceNetAdhocPtpStat *)malloc(sizeof(SceNetAdhocPtpStat));
								
								// Allocated Memory
								if(internal != NULL)
								{
									// Clear Memory
									memset(internal, 0, sizeof(SceNetAdhocPtpStat));
									
									// Copy Infrastructure Socket ID
									internal->id = socket;
									
									// Copy Address Information
									internal->laddr = *saddr;
									internal->lport = sport;
									
									// Flag Socket as Listener
									internal->state = PTP_STATE_LISTEN;
									
									// Set Buffer Size
									internal->rcv_sb_cc = bufsize;
									
									// Append to internal PTP Socket List
									_ptpAppendInternal(internal);
									
									// Return PTP Socket Pointer
									return (int)internal;
								}
							}
						}
						
						// Close Socket
						sceNetInetClose(socket);
					}
					
					// Socket not available
					return ADHOC_SOCKET_ID_NOT_AVAIL;
				}
				
				// Invalid Arguments
				return ADHOC_INVALID_ARG;
			}
			
			// Invalid Ports
			return ADHOC_PORT_IN_USE;
		}
		
		// Invalid Addresses
		return ADHOC_INVALID_ADDR;
	}
	
	// Library is uninitialized
	return ADHOC_NOT_INITIALIZED;
}
