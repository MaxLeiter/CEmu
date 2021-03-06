#include <stdio.h>

#include "apb.h"
#include "mem.h"
#include "emu.h"
#include "debug/debug.h"

/* Global APB state */
apb_map_entry_t apb_map[0x10];

void apb_set_map(int entry, eZ80portrange_t *range){
    apb_map[entry].range = range;
}

uint8_t port_read_byte(const uint16_t addr) {
    uint16_t port = (port_range(addr) << 12) | addr_range(addr);
#ifdef DEBUG_SUPPORT
    if (debugger.data.ports[port] & DBG_PORT_READ) {
        open_debugger(HIT_PORT_READ_BREAKPOINT, port);
    }
#endif
    return apb_map[port_range(addr)].range->read_in(addr_range(addr));
}

void port_write_byte(const uint16_t addr, const uint8_t value) {
    uint16_t port = (port_range(addr) << 12) | addr_range(addr);

#ifdef DEBUG_SUPPORT
    if (debugger.data.ports[port] & DBG_PORT_FREEZE) {
        return;
    }
#endif

    apb_map[port_range(addr)].range->write_out(addr_range(addr), value);

#ifdef DEBUG_SUPPORT
    if (debugger.data.ports[port] & DBG_PORT_WRITE) {
        open_debugger(HIT_PORT_WRITE_BREAKPOINT, port);
    }
#endif
}
