#include <cpu.h>
#include <bus.h>
#include <emu.h>


static cpu_context ctx;

void fetch_data()
{
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;
    if(ctx.current_instructon == NULL)
    {
        //printf("Unknown instruciton! %02X\n", ctx.cur_opcode);
        return;
    }
    switch (ctx.current_instructon->mode)
    {
    case AM_IMP: return;
    
    case AM_R: 
        ctx.fetched_data = cpu_read_reg(ctx.current_instructon->reg_1);
        return;
    case AM_R_R: 
        ctx.fetched_data = cpu_read_reg(ctx.current_instructon->reg_2);
        return;
    
    case AM_R_D8:
        ctx.fetched_data = bus_read(ctx.regs.pc);
        emu_cycles(1);
        ctx.regs.pc++;
        return;
    
    case AM_R_D16: 
    case AM_D16:{

        u16 lo = bus_read(ctx.regs.pc);
        emu_cycles(1);
        
        u16 high = bus_read(ctx.regs.pc+1);
        emu_cycles(1);
        

        ctx.fetched_data = lo | (high << 8); // combining two 8 bit registers into 1 16 bit

        ctx.regs.pc += 2;
        return;

    }

    case AM_MR_R:
    {
        ctx.fetched_data = cpu_read_reg(ctx.current_instructon->reg_2);
        ctx.mem_dest = cpu_read_reg(ctx.current_instructon->reg_1);
        ctx.dest_is_mem = true;

        if (ctx.current_instructon->reg_1 == RT_C)
        {
            ctx.mem_dest |= 0xFF00; // writes to reg c with the most significant bits

        }
        return;
    }

    case AM_R_MR:
    {
        u16 addr = cpu_read_reg(ctx.current_instructon->reg_2);

        if (ctx.current_instructon->reg_1 == RT_C)
        {
            ctx.mem_dest |= 0xFF00; // writes to reg c with the most significant bits

        }

        ctx.fetched_data = bus_read(addr);
        emu_cycles(1);

        return;

    }

    case AM_R_HLI:
    {
        ctx.fetched_data = bus_read(cpu_read_reg(ctx.current_instructon->reg_2));
        emu_cycles(1);
        cpu_set_reg(RT_HL, cpu_read_reg(RT_HL)+1);

        return;
    }
    case AM_R_HLD:
    {
        ctx.fetched_data = bus_read(cpu_read_reg(ctx.current_instructon->reg_2));
        emu_cycles(1);
        cpu_set_reg(RT_HL, cpu_read_reg(RT_HL)-1);

        return;
    }
    case AM_HLI_R:
    {
        ctx.fetched_data = cpu_read_reg(ctx.current_instructon->reg_2);
        ctx.mem_dest = cpu_read_reg(ctx.current_instructon->reg_1);
        ctx.dest_is_mem = true;
        cpu_set_reg(RT_HL, cpu_read_reg(RT_HL)+1);
        return;
    }
    case AM_HLD_R:
    {
        ctx.fetched_data = cpu_read_reg(ctx.current_instructon->reg_2);
        ctx.mem_dest = cpu_read_reg(ctx.current_instructon->reg_1);
        ctx.dest_is_mem = true;
        cpu_set_reg(RT_HL, cpu_read_reg(RT_HL)-1);
        return;
    }

    
    case AM_R_A8:
    {
        ctx.fetched_data = bus_read(ctx.regs.pc);
        emu_cycles(1);
        ctx.regs.pc++;
        return;
    }

    case AM_A8_R:
    {
        ctx.mem_dest = bus_read(ctx.regs.pc) | 0xFF00;
        ctx.dest_is_mem = true;
        emu_cycles(1);
        ctx.regs.pc++;
        return;


    }

    case AM_HL_SPR:
    {
        ctx.fetched_data = bus_read(ctx.regs.pc);
        emu_cycles(1);
        ctx.regs.pc++;
        return;

    }

    case AM_D8:
    {
        ctx.fetched_data = bus_read(ctx.regs.pc);
        emu_cycles(1);
        ctx.regs.pc++;
        return;
    }

    case AM_A16_R:
    case AM_D16_R:
    {
        u16 lo = bus_read(ctx.regs.pc);
        emu_cycles(1);
        
        u16 high = bus_read(ctx.regs.pc+1);
        emu_cycles(1);
        

        ctx.mem_dest = lo | (high << 8); // combining two 8 bit registers into 1 16 bit
        ctx.dest_is_mem = true;

        ctx.regs.pc += 2;
        ctx.fetched_data = cpu_read_reg(ctx.current_instructon->reg_2);
        return;
    }

    case AM_MR_D8:
    {
        ctx.fetched_data = bus_read(ctx.regs.pc);
        emu_cycles(1);
        ctx.regs.pc++;
        ctx.mem_dest = cpu_read_reg(ctx.current_instructon->reg_1);
        ctx.dest_is_mem = true;
        return;
    }
    
    case AM_MR:
    {
        
        
        ctx.mem_dest = cpu_read_reg(ctx.current_instructon->reg_1);
        ctx.dest_is_mem = true;
        ctx.fetched_data = bus_read(cpu_read_reg(ctx.current_instructon->reg_1));
        emu_cycles(1);
        return;
    }

    
    case AM_R_A16:
    {
        u16 lo = bus_read(ctx.regs.pc);
        emu_cycles(1);
        
        u16 high = bus_read(ctx.regs.pc+1);
        emu_cycles(1);
        

        u16 addr = lo | (high << 8); // combining two 8 bit registers into 1 16 bit

        ctx.regs.pc += 2;
        ctx.fetched_data = bus_read(addr);
        emu_cycles(1);
        return;

        
    }
    
    default:
        printf("Unknown addressing mode: %d\n (%02X)\n", ctx.current_instructon->mode, ctx.cur_opcode);
        exit(-7);
        return;
    }

}