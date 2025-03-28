#include <pcap.h>
#include <stdio.h>

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
   printf("Got a packet\n");
}

int main()
{
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program fp;
//  char filter_exp[] = "icmp[icmptype]=0";
  char filter_exp[] = "proto \\tcp";
  bpf_u_int32 net;
  
  int e;
  pcap_t *p;
  char a;

  // Step 1: Open live pcap session on NIC with name enp0s3
  handle = pcap_open_live("wlp3s0", BUFSIZ, 1, 1000, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "Can't open wlp3s0: %s\n", errbuf);
    exit(1);
  }

  // Step 2: Compile filter_exp into BPF psuedo-code
  e = pcap_compile(handle, &fp, filter_exp, 0, net);              
  e = pcap_setfilter(handle, &fp);
                            
  // Step 3: Capture packets
  pcap_loop(handle, -1, got_packet, NULL);                    

  pcap_close(handle);   //Close the handle
  return 0;
}

