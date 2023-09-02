#include <cpu.h>
#include <bus.h>
#include <emu.h>
cpu_context ctx = {0};

void cpu_init()
{
    ctx.regs.pc = 0x100;
    //ctx.regs.a = 0x01;

}


static void fetch_instruction()
{
    ctx.cur_opcode = bus_read(ctx.regs.pc++);
    ctx.current_instructon = instruction_by_opcode(ctx.cur_opcode);

    if(ctx.current_instructon == NULL)
    {
        printf("Unknown instruciton! %02X\n", ctx.cur_opcode);
        exit(-7);
    }

}

static void fetch_data()
{
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    switch (ctx.current_instructon->mode)
    {
    case AM_IMP: return;
    
    case AM_R: 
        ctx.fetched_data = cpu_read_reg(ctx.current_instructon->reg_1);
        return;
    
    case AM_R_D8:
        ctx.fetched_data = bus_read(ctx.regs.pc);
        emu_cycles(1);
        ctx.regs.pc++;
        return;
    case AM_D16:{

        u16 lo = bus_read(ctx.regs.pc);
        emu_cycles(1);
        
        u16 high = bus_read(ctx.regs.pc+1);
        emu_cycles(1);
        

        ctx.fetched_data = lo | (high << 8); // combining two 8 bit registers into 1 16 bit

        ctx.regs.pc += 2;
        return;

    }

    default:
        printf("Unknown addressing mode: %d\n", ctx.current_instructon->mode);
        exit(-7);
        return;
    }

}

static void excecute()
{
    
    printf("\tNot excecuting yet \n");

}


bool cpu_step()
{
    
    if(!ctx.isHalted)
    {
        u16 pc = ctx.regs.pc;
        fetch_instruction();
        fetch_data();
        printf("Exceuting Instruciton %02X     PC: %04X\n", ctx.cur_opcode, pc);
        excecute();
    }
    return true;
}