#include <iostream>
#include <fstream> 
#include <stdio.h>
#include <stdint.h>

using namespace std;

uint32_t little_endian_uint32(uint8_t buff[]) {
	return (uint32_t)(
			(buff[3]) |
			(buff[2] << 8) |
			(buff[1] << 16) |
			(buff[0] << 24)
		);
}

struct ts_packet {
	uint8_t 	sync;
	bool		tei;
	bool		pusi;
	bool		trans_priority;
	uint16_t 	pid;
	uint8_t		tsc;
	uint8_t		adap_field;
	uint8_t		cont_count;
};

void pack_dump(ts_packet &packet) {
	cout << "\n==================\n";
	printf("Sync:	%x\n",packet.sync);
	printf("TEI:	%d\n",packet.tei);
	printf("PUSI:	%d\n",packet.pusi);
	printf("t_pri:	%d\n",packet.trans_priority);
	printf("pid:	%d\n",packet.pid);
	printf("tsc:	%d\n",packet.tsc);
	printf("adp:	%d\n",packet.adap_field);
	printf("len:	%d\n",packet.cont_count);
	cout << "=================\n";
}

void pack_header(ts_packet &packet, uint32_t header) {
	packet.sync = (header & 0xff000000) >> 24;
	packet.tei = (header & 0x800000) >> 23;
	packet.pusi = (header & 0x400000) >> 22;
	packet.trans_priority = (header & 0x200000) >> 21;
	packet.pid = (header & 0x1fff00) >> 8;
	packet.tsc = (header & 0xc0) >> 6;
	packet.adap_field = (header & 0x30) >> 4;
	packet.cont_count = header & 0xf;
}

void parse_packet(ifstream &ts) {
	ts_packet packet;
	uint8_t ts_header[4] = {0x47,0,0,0};
        // Stream until we get the correct sync value
        while(ts.get() != 0x47);
        // Now lets read the next 3 bytes
        ts.read((char *)(&ts_header[1]),3);
        uint32_t header = little_endian_uint32(ts_header);
        pack_header(packet, header);
        pack_dump(packet);
}

int main(int argc, char *argv[]) {
	// Open input filestream
	ifstream ts ("test.ts", ifstream::in);
	// Get length of file
	ts.seekg(0,ts.end);
	int length = ts.tellg();
	ts.seekg(0,ts.beg);
	for (int i = 0; i < 10; i++) {
		parse_packet(ts);
	}
}
