#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");
    
    // initialuise simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for (i = 0; i<300; ++i)
    {
        // dump variables into VCD file and toggle clock
        for (clk = 0; clk<2; ++clk)
        {
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }
        
        // challenge 1 - stop counting for 3 cycles once the counter reaches 0x9, and then resume counting i.e do not reset after 9
        top->rst = (i<2);
        // disable counter for i<4 and 15<i<17 - i.e enable counter for 4<i<15 and i >17
        top->en = (((i>4) && (i<14)) || (i>1));

        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}