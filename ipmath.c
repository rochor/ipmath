/*
 * ipmath.c
 *
 *  Created on: Oct 13, 2019
 *      Author: juha
 */
#include <stdio.h>
#include <string.h>

const int PREFIX_LEN = 16;
const int leadingzero = 0;

unsigned int get_ip_integral_equivalent(char *ip_address){
	unsigned int ip_equivalent = 0;
	unsigned int octet = 0;
	unsigned int octet_num = 3;
	int i = 0;
	while (ip_address[i] != '\0') {
		if(ip_address[i] == '.') {
			ip_equivalent += octet << (8*octet_num);
			octet_num--;
			octet = 0;
		}
		else {
			octet *= 10;
			octet += (int)ip_address[i] - 48;
		}
		i++;
	}
	ip_equivalent += octet;
	return ip_equivalent;
}

void get_abcd_ip_format(unsigned int ip_equivalent, char *ip_addr) {
	int octet_num = 3;
	unsigned int octet = 0;
	int i = 0;
	int num1 = 0;
	int num2 = 0;
	int num3 = 0;
	while(octet_num >= 0) {
		octet = ip_equivalent >> (8*3);
		num3 = octet % 10;
		octet /= 10;
		num2 = octet %10;
		octet /= 10;
		num1 = octet;
		if(num1 == 0) {
			if(num2 == 0) {
				ip_addr[i] = num3 + 48;
				i++;
			}
			else {
				ip_addr[i] = num2 + 48;
				i++;
				ip_addr[i] = num3 + 48;
				i++;
			}
		}
		else{
			ip_addr[i] = num1 + 48;
			i++;
			ip_addr[i] = num2 + 48;
			i++;
			ip_addr[i] = num3 + 48;
			i++;
		}
		if(octet_num > 0) {
			ip_addr[i] = '.';
			i++;
		}
		octet_num--;
		octet = 0;
		num1 = 0;
		num2 = 0;
		num3 = 0;
		ip_equivalent = ip_equivalent << 8;
	}
}

void get_network_id(char *ip_addr, char mask, char *output_buffer){
	unsigned int ip_eq = get_ip_integral_equivalent(ip_addr);
	unsigned int subnet = ~(0xFFFFFFFF >> (int)mask);
	ip_eq &= subnet;
	get_abcd_ip_format(ip_eq, output_buffer);
}

void get_broadcast_address(char *ip_addr, char mask, char *output_buffer){
	unsigned int ip_eq = get_ip_integral_equivalent(ip_addr);
	unsigned int broadcast = 0xFFFFFFFF >> (int)mask;
	ip_eq |= broadcast;
	get_abcd_ip_format(ip_eq, output_buffer);
}

unsigned int get_subnet_cardinality(char mask) {
	return (0xFFFFFFFF >> (int)mask) -1;
}

int check_ip_subnet_membership(char *network_id, char mask, char *check_ip) {
	char check_ip_net[PREFIX_LEN];
	memset(check_ip_net,0,PREFIX_LEN);
	get_network_id(check_ip,mask,check_ip_net);
	//printf("%s %s\n", check_ip_net, network_id);
	if(strcmp(network_id, check_ip_net) == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

int main(int argc, char **argv) {
	/*IP address conversion demo*/
	char *ip_address = "10.1.23.10";
	unsigned int int_ip = get_ip_integral_equivalent(ip_address);
	printf("Integer equivalent to IP %s is %u\n", ip_address, int_ip);

	/*ABCD IP addr demo*/
	unsigned int ip = 2058138165;
	char ipadd_buffer[PREFIX_LEN];
	memset(ipadd_buffer,0,PREFIX_LEN);
	get_abcd_ip_format(ip, ipadd_buffer);
	printf("IP %u in A.B.C.D format is %s\n", ip, ipadd_buffer);

	/*broadcast addr demo*/
	memset(ipadd_buffer,0,PREFIX_LEN);
	char *ip_add = "10.1.23.10";
	char mask = 20;
	get_broadcast_address(ip_add,mask,ipadd_buffer);
	printf("IP %s broadcast address is %s\n", ip_add, ipadd_buffer);

	/*network ID demo*/
	memset(ipadd_buffer,0,PREFIX_LEN);
	char *ip_add2 = "192.168.2.10";
	char mask2 = 20;
	get_network_id(ip_add2,mask2,ipadd_buffer);
	printf("IP %s network ID is %s\n", ip_add, ipadd_buffer);

	/*Subnet cardinality demo*/
	char mask3 = 24;
	printf("Subnet cardinality for Mask = %u is %u\n", mask3, get_subnet_cardinality(mask3));

	/*Subnet membership demo*/
	char *network_id = "192.168.0.0";
	char mask4 = 24;
	char *check_ip = "192.168.0.13";
	int result = check_ip_subnet_membership(network_id, mask4, check_ip);
	if(result == 0)
		printf("IP address = %s is a member of subnet %s/%u\n", check_ip, network_id, mask4);
	else
		printf("IP address = %s is not a member of subnet %s/%u\n", check_ip, network_id, mask4);
}

