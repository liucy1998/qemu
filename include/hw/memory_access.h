/*
 * This file is part of Redqueen.
 *
 * Sergej Schumilo, 2019 <sergej@schumilo.de>
 * Cornelius Aschermann, 2019 <cornelius.aschermann@rub.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef MEMORY_ACCESS_H
#define MEMORY_ACCESS_H

#include "qemu/osdep.h"
#include <linux/kvm.h>
#include "qemu-common.h"
#include "sysemu/kvm_int.h"

#define x86_64_PAGE_SIZE	0x1000
#define x86_64_PAGE_MASK	~(x86_64_PAGE_SIZE - 1)

static bool read_virtual_memory(uint64_t address, uint8_t* data, uint32_t size, CPUState *cpu){
	uint8_t tmp_buf[x86_64_PAGE_SIZE];
	MemTxAttrs attrs;
	hwaddr phys_addr;
	int asidx;
	bool ret = true;
  uint64_t amount_copied = 0;
	
	//cpu_synchronize_state(cpu);
	kvm_cpu_synchronize_state(cpu);

	/* copy per page */
	while(amount_copied < size){
		uint64_t len_to_copy = (size - amount_copied);
    if(len_to_copy > x86_64_PAGE_SIZE)
      len_to_copy = x86_64_PAGE_SIZE;

		asidx = cpu_asidx_from_attrs(cpu, MEMTXATTRS_UNSPECIFIED);
		attrs = MEMTXATTRS_UNSPECIFIED;
		phys_addr = cpu_get_phys_page_attrs_debug(cpu, (address & x86_64_PAGE_MASK), &attrs);

    if (phys_addr == -1){
      uint64_t next_page = (address & x86_64_PAGE_MASK) + x86_64_PAGE_SIZE;
      uint64_t len_skipped =next_page-address;  
      if(len_skipped > size-amount_copied){
        len_skipped = size-amount_copied;
      }

      printf("Warning, read from unmapped memory: %lx, skipping to %lx", address, next_page);
	  memset( data+amount_copied, 0x90,  len_skipped); // fill with NOPs
      address += len_skipped;
      amount_copied += len_skipped;
      ret = false;
      continue;
    }
		
		phys_addr += (address & ~x86_64_PAGE_MASK);
    uint64_t remaining_on_page = x86_64_PAGE_SIZE - (address & ~x86_64_PAGE_MASK);
    if(len_to_copy > remaining_on_page){
      len_to_copy = remaining_on_page;
    }
		MemTxResult txt = address_space_rw(cpu_get_address_space(cpu, asidx), phys_addr, MEMTXATTRS_UNSPECIFIED, tmp_buf, len_to_copy, 0);
    if(txt){
      printf("Warning, read failed:\t%lx", address);
    }
		
		memcpy(data+amount_copied, tmp_buf, len_to_copy);
		
		address += len_to_copy;
		amount_copied += len_to_copy;
	}
	
	return ret;
}

// static bool is_addr_mapped(uint64_t address, CPUState *cpu){
// 	MemTxAttrs attrs;
// 	hwaddr phys_addr;
// 	kvm_cpu_synchronize_state(cpu);
// 	attrs = MEMTXATTRS_UNSPECIFIED;
// 	phys_addr = cpu_get_phys_page_attrs_debug(cpu, (address & x86_64_PAGE_MASK), &attrs);
// 	return phys_addr != -1;
// }

static bool write_virtual_memory(uint64_t address, uint8_t* data, uint32_t size, CPUState *cpu)
{
	int asidx;
	MemTxAttrs attrs;
    hwaddr phys_addr;
    MemTxResult res;

    uint64_t counter, l, i;

    counter = size;
	while(counter != 0){
		l = x86_64_PAGE_SIZE;
        if (l > counter)
            l = counter;

	kvm_cpu_synchronize_state(cpu);
        //cpu_synchronize_state(cpu);
        asidx = cpu_asidx_from_attrs(cpu, MEMTXATTRS_UNSPECIFIED);
        attrs = MEMTXATTRS_UNSPECIFIED;
        phys_addr = cpu_get_phys_page_attrs_debug(cpu, (address & x86_64_PAGE_MASK), &attrs);

        if (phys_addr == -1){
        	printf("phys_addr == -1:\t%lx", address);
            return false;
        }
        
        phys_addr += (address & ~x86_64_PAGE_MASK);   
        res = address_space_rw(cpu_get_address_space(cpu, asidx), phys_addr, MEMTXATTRS_UNSPECIFIED, data, l, true);
        if (res != MEMTX_OK){
        	printf("!MEMTX_OK:\t%lx", address);
            return false;
        }   

        i++;
        data += l;
        address += l;
        counter -= l;
	}

	return true;
}
#endif