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