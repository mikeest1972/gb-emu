#include <cpu.h>
#include <bus.h>
#include <emu.h>
cpu_context ctx = {0};

void cpu_init()
{
    ctx.regs.pc = 0x100;
    ctx.regs.a = 0x01;

}


static void fetch_instruction()
{
    ctx.cur_opcode = bus_read(ctx.regs.pc++);
    ctx.current_instructon = instruction_by_opcode(ctx.cur_opcode);

    

}

static void fetch_data()
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
        printf("Unknown addressing mode: %d\n (%02X)\n", ctx.current_instructon->mode, ctx.cur_opcode);
        exit(-7);
        return;
    }

}

static void excecute()
{
    
    IN_PROC proc = inst_get_processor(ctx.current_instructon->type);

    if(!proc)
    {
        NO_IMPL
    }
    proc(&ctx);

}


bool cpu_step()
{
    
    if(!ctx.isHalted)
    {
        u16 pc = ctx.regs.pc;
        fetch_instruction();
        fetch_data();
         printf("%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X \n",
        pc, 
        inst_name(ctx.current_instructon->type),
        bus_read(pc+1),
        bus_read(pc+2),
        ctx.regs.a,
        ctx.regs.b,
        ctx.regs.c
        );
       
        if(ctx.current_instructon == NULL)
        {
            printf("Unknown instruciton! %02X\n", ctx.cur_opcode);
            exit(-7);
        }
        excecute();
    }
    return true;
}