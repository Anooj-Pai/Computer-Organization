module ALUControl (ALUOp, Funct, ALUCtl);
      always @(*) begin
            $display("ALUControl:");
            $display("  INPUTS:");
            $display("        ALUOp: %2b", ALUOp);
            $display("        Funct: %6b", Funct);
      end

      input [1:0] ALUOp;
      input [5:0] Funct;
      output reg [3:0] ALUCtl;
      
      always @(*) begin
            case (ALUOp)
            2'b00: ALUCtl = 4'b0010; // add
            2'b01: ALUCtl = 4'b0110; // sub
            2'b10: ALUCtl = 4'b0110; // sub
            2'b11: ALUCtl = 4'b0110; // sub
            endcase
            $display("  OUTPUTS:");
            $display("        ALUCtl: %4b\n\n", ALUCtl);
      end
endmodule

module Mult3to1(ALUResultOut, ALUOut, JumpAddr, PCSource, PCValue);
      always @(*) begin
            $display("Mult3to1:");
            $display("  INPUTS:");
            $display("        ALUResultOut: %1d", ALUResultOut);
            $display("        ALUOut: %1d", ALUOut);
            $display("        JumpAddr: %1d", JumpAddr);
            $display("        PCSource: %2b", PCSource);
      end

      input [31:0] ALUResultOut, ALUOut, JumpAddr;
      input [1:0] PCSource;
      output reg [31:0] PCValue;
       
      always @(*) begin
            case (PCSource)
            2'b00: PCValue = ALUResultOut;
            2'b01: PCValue = ALUOut;
            2'b10: PCValue = JumpAddr;
            2'b11: PCValue = JumpAddr;
            endcase
            $display("  OUTPUTS:");
            $display("        PCValue: %1d\n\n", PCValue);
      end
endmodule

module Mult4to1(B, constant4, SignExtendOffset, PCOffset, ALUSrcB, ALUBin);
      always @(*) begin
            $display("Mult4to1:");
            $display("  INPUTS:");
            $display("        B: %1d", B);
            $display("        constant4: %1d", constant4);
            $display("        SignExtendOffset: %1d", SignExtendOffset);
            $display("        PCOffset: %1d", PCOffset);
            $display("        ALUSrcB: %2b", ALUSrcB);
      end

      input [31:0] B, constant4, SignExtendOffset, PCOffset;
      input [1:0] ALUSrcB;
      output reg [31:0] ALUBin;
       
      always @(*) begin
            case (ALUSrcB)
            2'b00: ALUBin = B;
            2'b01: ALUBin = constant4;
            2'b10: ALUBin = SignExtendOffset;
            2'b11: ALUBin = PCOffset;
            endcase
            $display("  OUTPUTS:");
            $display("        ALUBin: %1d\n\n", ALUBin);
      end

endmodule

module MIPSALU(ALUCtl, ALUAin, ALUBin, ALUResultOut, Zero);

      always @(*) begin
            $display("ALU:");
            $display("  INPUTS:");
            $display("        ALUCtl: %4b", ALUCtl);
            $display("        ALUAin: %1d", ALUAin);
            $display("        ALUBin: %1d", ALUBin);
      end
      
      input [3:0] ALUCtl;
      input [31:0] ALUAin, ALUBin;
      output reg [31:0] ALUResultOut;
      output Zero;
       
      always @(*) begin
            case (ALUCtl)
            4'b0010: ALUResultOut = ALUAin + ALUBin;
            4'b0110: ALUResultOut = ALUAin - ALUBin;
            4'b0111: ALUResultOut = ALUAin - ALUBin;
            endcase
            $display("  OUTPUTS:");
            $display("        ALUResultOut: %1d\n\n", ALUResultOut);
      end
endmodule

// registerfile regs (IR[25:21], IR[20:16], Writereg, Writedata, RegWrite, A, B, clock);
module registerfile(rs, rt, Writereg, Writedata, RegWrite, A, B, clock);
      input [4:0] rs, rt, Writereg;
      input [31:0] Writedata;
      input RegWrite, clock;
      output reg [31:0] A, B;
      reg [31:0] regfile [0:31];
      initial regfile[2] = 1;
      initial regfile[3] = 0;
      initial regfile[4] = 2;
      initial regfile[5] = 3;
       
      always @(posedge clock) begin
            $display("registerfile:");
            $display("  INPUTS:");
            $display("        rs: %5b", rs);
            $display("        rt: %5b", rt);
            $display("        Writereg: %5b", Writereg);
            $display("        Writedata: %1d", Writedata);
            $display("        RegWrite: %1d", RegWrite);
            $display("        clock: %1d", clock);
            if (RegWrite) begin
            regfile[Writereg] <= Writedata;
            end
            A <= regfile[rs];
            B <= regfile[rt];
            $display("  OUTPUTS:");
            $display("        A: %1d", A);
            $display("        B: %1d\n\n", B);
      end
endmodule

module Datapath (ALUOp, RegDst, MemtoReg, MemRead, MemWrite, IorD, RegWrite, IRWrite,
                 PCWrite, PCWriteCond, ALUSrcA, ALUSrcB, PCSource, opcode, clock); // the control inputs + clock

   input [1:0]  ALUOp, ALUSrcB, PCSource;                   // 2-bit control signals
   input        RegDst, MemtoReg, MemRead, MemWrite, IorD,
                RegWrite, IRWrite, PCWrite, PCWriteCond,
                ALUSrcA, clock;                             // 1-bit control signals
   output [5:0] opcode;                                     // opcode is needed as an output by control
   reg [31:0]   PC, Memory [0:1023], MDR, ALUOut;       // CPU state + some temporaries
   reg [31:0]   IR;
   //reg [31:0] IR = 32'b10001100100000100000000000000000; // lw $2, 0($3)
   wire [31:0]  A, B, SignExtendOffset, PCOffset,
                ALUResultOut, PCValue, JumpAddr,
                Writedata, ALUAin, ALUBin, MemOut;          // these are signals derived from registers
   wire [3:0]   ALUCtl;                                     // the ALU control lines
   wire         Zero;                                       // the Zero out signal from the ALU
   wire[4:0]    Writereg;                                   // the signal used to communicate the destination register
   
   reg [1:0]    DatapathState;
 
   initial PC = 0;              //start the PC at 0
   initial DatapathState = 0;   // start the state at 0
   initial IR = 32'b10101100100000100000000000000000; // sw $2, 0($3)


   // Combinational signals used in the datapath
   // Read using word address with either ALUOut or PC as the address source
   assign MemOut = MemRead ? Memory[(IorD ? ALUOut : PC) >> 2]:0;
   assign opcode = IR[31:26];                             // opcode shortcut
 
   // Get the write register address from one of two fields depending on RegDst
   assign Writereg = RegDst ? IR[15:11] : IR[20:16];
 
   // Get the write register data either from the ALUOut or from the MDR
   assign Writedata = MemtoReg ? MDR : ALUOut;
 
   // Sign-extend the lower half of the IR from load/store/branch offsets
   assign SignExtendOffset = {{16{IR[15]}}, IR[15:0]};    // sign-extend lower 16 bits;
 
   // The branch offset is also shifted to make it a word offset
   assign PCOffset = SignExtendOffset << 2;
 
   // The A input to the ALU is either the rs register or the PC
   assign ALUAin = ALUSrcA ? A : PC;                      // ALU input is PC or A
 
   // Compose the Jump address
   assign JumpAddr = {PC[31:28], IR[25:0], 2'b00};        // The jump address
 
 
   // Creates an instance of the ALU control unit (see the module defined in
   // COD Figure B.5.16 (The MIPS ALU Control: a simple piece of combinational logic)
         // Input ALUOp is control-unit set and used to describe the instruction class
         // as in COD Chapter 4 (The Processor)
         // Input IR[5:0] is the function code field for an ALU instruction
         // Output ALUCtl are the actual ALU control bits as in Chapter 4
   ALUControl alucontroller (ALUOp, IR[5:0], ALUCtl); //ALU control unit
 
   // Creates a 3-to-1 multiplexor used to select the source of the next PC
         // Inputs are ALUResultOut (the incremented PC) , ALUOut (the branch address), the jump target address
         // PCSource is the selector input and PCValue is the multiplexor output
   Mult3to1 PCdatasrc (ALUResultOut, ALUOut, JumpAddr, PCSource, PCValue);
 
   // Creates a 4-to-1 multiplexor used to select the B input of the ALU
         // Inputs are register B,constant 4, sign-extended lower half of IR, sign-extended lower half of IR << 2
         // ALUSrcB is the selector input
         // ALUBin is the multiplexor output
   Mult4to1 ALUBinput (B, 32'd4, SignExtendOffset, PCOffset, ALUSrcB, ALUBin);
 
   // Creates a MIPS ALU
         // Inputs are ALUCtl (the ALU control), ALU value inputs (ALUAin, ALUBin)
         // Outputs are ALUResultOut (the 32-bit output) and Zero (zero detection output)
   MIPSALU ALU (ALUCtl, ALUAin, ALUBin, ALUResultOut, Zero); // the ALU
 
   // Creates a MIPS register file
         // Inputs are
         // the rs and rt fields of the IR used to specify which registers to read,
         // Writereg (the write register number), Writedata (the data to be written), RegWrite (indicates a write), the clock
         // Outputs are A and B, the registers read
   registerfile regs (IR[25:21], IR[20:16], Writereg, Writedata, RegWrite, A, B, clock); //Register file
 
   // The clock-triggered actions of the datapath
   always @(posedge clock) begin  
      if (DatapathState == 0) begin
            DatapathState <= 1;
      end else if(DatapathState == 1) begin
            IR <= 32'b00000000100001010011000000100000; // add $2, $4, $5
            DatapathState <= 2; // move to next state
      end else if(DatapathState == 2) begin
            IR <= 32'b10001100100000100000000000000000; // lw $2, 0($3)
            DatapathState <= 3;
      end else begin
            $display("Simulation Done\n");
            $finish;
      end
      
      
      if (MemWrite)
         Memory[ALUOut >> 2] <= B;        // Write memory--must be a store
 
      ALUOut <= ALUResultOut;             // Save the ALU result for use on a later clock cycle
 
      if (IRWrite)                   // Write the IR if an instruction fetch
         IR <= MemOut;
 
      MDR <= MemOut;                 // Always save the memory read value
 
      // The PC is written both conditionally (controlled by PCWrite) and unconditionally
      if (PCWrite || (PCWriteCond & Zero))
         PC <= PCValue;
   end
endmodule


module CPU (clock);
   parameter LW  = 6'b100011,
             SW  = 6'b101011,
             BEQ = 6'b000100,
             J   = 6'd2;           // constants
   
 
   input clock;
   reg [2:0] state;
   wire [1:0] ALUOp, ALUSrcB, PCSource;
   wire [5:0] opcode;
   wire RegDst, MemRead, MemWrite, IorD, RegWrite, IRWrite, PCWrite, PCWriteCond,
        ALUSrcA, MemoryOp, IRWwrite, Mem2Reg;
   reg [1:0] CPUState;

   initial CPUState = 0;

   // assign the control signals to the datapath
   assign IRWrite = 1;
   assign Mem2Reg = 0;
   assign MemoryOp = 0;
   assign ALUOp = 2'b10;
   assign RegDst = 0;
   assign MemRead = 0;
   assign MemWrite = 1;
   assign IorD = 0;
   assign RegWrite = 0;
   assign PCWrite = 0;     
   assign PCWriteCond = 0;
   assign ALUSrcA = 0;
   assign ALUSrcB = 1;
   assign PCSource = 0;
 
   // Create an instance of the MIPS datapath, the inputs are the control signals;
   // opcode is only output
   Datapath MIPSDP (ALUOp, RegDst, Mem2Reg, MemRead, MemWrite, IorD, RegWrite,
                    IRWrite, PCWrite, PCWriteCond, ALUSrcA, ALUSrcB, PCSource, opcode, clock);


      // always @(posedge clock) begin
      //       if(CPUState == 0) begin
      //             CPUState <= 1;
      //       end else if(CPUState == 1) begin
      //             CPUState <= 2;
      //             assign IRWrite = 1;
      //             assign Mem2Reg = 0;
      //             assign MemoryOp = 0;
      //             assign ALUOp = 2'b10;
      //             assign RegDst = 1;
      //             assign MemRead = 0;
      //             assign MemWrite = 0;
      //             assign IorD = 0;
      //             assign RegWrite = 1;
      //             assign PCWrite = 0;     
      //             assign PCWriteCond = 0;
      //             assign ALUSrcA = 0;
      //             assign ALUSrcB = 0;
      //             assign PCSource = 0;
      //       end else if(CPUState == 2) begin
      //             assign IRWrite = 1;
      //             assign Mem2Reg = 1;
      //             assign MemoryOp = 0;
      //             assign ALUOp = 2'b10;
      //             assign RegDst = 0;
      //             assign MemRead = 1;
      //             assign MemWrite = 0;
      //             assign IorD = 0;
      //             assign RegWrite = 1;
      //             assign PCWrite = 0;     
      //             assign PCWriteCond = 0;
      //             assign ALUSrcA = 0;
      //             assign ALUSrcB = 1;
      //             assign PCSource = 0;
      //             CPUState <= 3;
      //       end 
      // end     
      
endmodule

module testbench();
      reg clock;
      initial begin
            clock = 0;
            forever begin
            #5 clock = ~clock;
            end
      end
      CPU cpu(clock);

endmodule
