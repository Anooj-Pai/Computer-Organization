// MUX
module Mux2to1 ();
    
endmodule

// ALU Control
module ALUControl(ALUOp, Funct, ALUCtl);
    
    input [1:0] ALUOp;
    input [5:0] Funct;
    output reg [3:0] ALUCtl;

    always @(ALUOp, Funct) begin
        case (ALUOp)
            2'b00: begin
                case (Funct)
                    6'b100000: ALUCtl <= 2; // add
                    6'b100010: ALUCtl <= 6; // subtract
                    default: ALUCtl <= 15; // should not happen
                endcase
            end
            2'b01: begin
                case (Funct)
                    6'b100000: ALUCtl <= 2; // add
                    6'b100010: ALUCtl <= 6; // subtract
                    default: ALUCtl <= 15; // should not happen
                endcase
            end
            2'b10: begin
                case (Funct)
                    6'b100000: ALUCtl <= 2; // add
                    6'b100010: ALUCtl <= 6; // subtract
                    default: ALUCtl <= 15; // should not happen
                endcase
            end
            2'b11: begin
                case (Funct)
                    6'b100000: ALUCtl <= 2; // add
                    6'b100010: ALUCtl <= 6; // subtract
                    default: ALUCtl <= 15; // should not happen
                endcase
            end
            default: ALUCtl <= 15; // should not happen
        endcase
    end


    /*
    input [1:0] ALUOp;
    input [5:0] Funct;
    output reg [3:0] ALUCtl;
    always case (Funct)
        32: ALUCtl <= 2; // add
        34: ALUCtl <= 6; //subtract
        default: ALUCtl <= 15; // should not happen
    endcase
    */
endmodule

// ALU
module ALU(ALUCtl, RD1, RD2, ALUOut);

    input [3:0] ALUCtl;
    input [31:0] RD1,RD2;
    output reg [31:0] ALUOut;

    always @(*) begin
        case (ALUCtl)
            4'b0010: assign ALUOut = RD1 + RD2; // ADD
            4'b0110: assign ALUOut = RD1 - RD2; // SUB
            default: ALUOut <= 0;
             // default to 0, should not happen
        endcase
    end
    
    
    /*
    input [3:0] ALUCtl;
    input [31:0] RD1,RD2;
    output wire [31:0] ALUOut;
    
    always @(ALUCtl, RD1, RD2) //reevaluate if these change
        case (ALUCtl)
            2: begin
                assign ALUOut = RD1 + RD2;
            end
            6: begin 
                assign ALUOut = RD1 - RD2;
            end
            default: ALUOut <= 0; //default to 0, should not happen;
        endcase
    */
endmodule

/*
//control signals
module Control(Funct, RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, ALUOp);
endmodule
*/

// Data Path
module DataPath (clk);

/*

    Variables:
        PC = stores address of current instruction
        RR1_R = read register 1 in register file
        RR2_R = read register 2 in register file
        WR = write register in register file
        WD_R = write data in register file
        Offset = offset in lw and sw instructions
        ext_Offset = offset extended to 32 bits for ALU
        Funct = funct part of the code for ALUControl
        ALUCtl = output of the ALUControl that goes into ALU
        RD_D = read data in the data memory unit
        ALUOutput = output of the ALU 
        Memory 

*/

// Initialize registers and wires
reg [31:0]PC;
reg [31:0]RR1_R;
reg [31:0]RR2_R;
reg [31:0]RD1;
reg [31:0]RD2;
reg [31:0]WR;
reg [31:0]WD_R;
reg [15:0]Offset;
reg [31:0]ext_Offset;
reg [5:0]Funct;
reg [3:0]ALUCtl;
reg [31:0]RD_D;
reg [31:0]ALUOutput;
reg [31:0]Memory[1023:0];
reg [31:0]Registers[31:0];
reg [31:0]Instruction;
reg [5:0]Control;

//controls
reg RegDst;
reg ALUSrc;
reg MemtoReg;
reg RegWrite;
reg MemRead;
reg MemWrite;
reg [1:0]ALUOp;


// Inputs
input clk;

initial begin
    PC = 0; //initialize PC to 0
end

// Initialize modules
ALUControl ALUControl_1(ALUOp, Funct, ALUCtl);
ALU ALU_1(ALUCtl, RD1, RD2, ALUOutput);

always @(clk) begin
    // Get instruction from memory
    Instruction <= Memory[PC >> 2];

    // Break up instruction into different parts
    // Assign RR1, RR2, WR and Offset
    Control <= Instruction[31:26];
    RR1_R <= Instruction[25:21];
    RR2_R <= Instruction[20:16];
    WR <= Instruction[15:11];
    Funct <= Instruction[5:0];
    Offset <= Instruction[15:0];

    // Assign RD1, RD2, and ext_Offset
    RD1 <= Registers[RR1_R];
    RD2 <= Registers[RR2_R];
    ext_Offset <= {16{Offset[15]}};

    // Assign Funct and put into ALUControl function
    Funct <= Instruction[5:0];

    // Do operations with D-Mem
    // case for lw command
    if (MemRead == 1) begin
        RD_D <= Memory[ALUOutput >> 2];
    // case for sw command
    end else if (MemWrite == 1) begin
        Memory[ALUOutput >> 2] <= ALUOutput;
    end

    // Assign WD_R
    if (MemtoReg == 1) begin
        WD_R <= RD_D;
    end else begin
        WD_R <= ALUOutput;
    end    

    // Increase PC value
    PC <= PC + 4;

end

endmodule
