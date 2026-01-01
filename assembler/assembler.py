#!/usr/bin/env python3

import sys
import re
import os
from pathlib import Path
from enum import Enum

class instruction(Enum):
    A_INSTRUCTION = 0
    C_INSTRUCTION = 1
    L_INSTRUCTION = 2 

class Parser:
    def __init__(self, file_path):
        self.instructions = []
        try:
            with open(file_path, 'r') as f:
                self.raw_lines = f.readlines()
            self._clean_code()
        except FileNotFoundError:
            print("Error: Parser could not find file.")

        self.current_inst_index = 0
        self.has_more_lines = True
        self.current_instruction_type = instruction.A_INSTRUCTION
        self.current_instruction = ""

    def _clean_code(self):
        for line in self.raw_lines:
            # 1. Remove inline comments and trailing/leading whitespace
            clean_line = line.split("//")[0].strip()
            
            # conditional ignores lines that are now empty
            if clean_line:
                # remove interior whitespace
                self.instructions.append(clean_line.replace(" ", ""))

    def advance(self): 
        self.current_index += 1
        if self.current_index >= len(self.instructions):
            self.has_more_lines = False
            return

        self.current_instruction = self.instructions[self.current_inst_index]
        # startswith() is safer than accessing by index as if the string is empty it can crash
        if self.current_instruction.startswith("@"):
            self.current_instruction_type = self.current_instruction.A_INSTRUCTION
        elif "=" in self.current_instruction or ";" in self.current_instruction:
            self.current_instruction_type = self.current_instruction.C_INSTRUCTION
        elif self.current_instruction.startswith("("):
            self.current_instruction_type = self.current_instruction.L_INSTRUCTION

class Code:
    def decode_dest(dest):
        match dest:
            case "M":
                return 
    
# D=A+1;JEQ
# comp = A+1, dest = D, jump = JEQ

if __name__ == "__main__":
        
    filename = input("Enter filename of .asm file in /programs: ").strip()

    current_dir = os.path.dirname(os.path.abspath(__file__))
    target_path = os.path.join(current_dir, "programs", filename)
    
    if os.path.exists(target_path):
        print(f"File found: {target_path}")
    else:
        print(f"Error: Could not find {filename} in 'programs' directory.")

    parser = Parser(path)
    code = Code()

    converted_instructions = []
    converted_instructions_index = 0

    if path:
        while parser.has_more_lines:
            parser.advance()

            if parser.current_instruction_type is instruction.A_INSTRUCTION:
                decimal_string = parser.current_instruction.replace("@", "")
                converted_instructions[converted_instructions_index] = bin(int(decimal_string))[2:]

            elif parser.current_instruction_type is instruction.C_INSTRUCTION:
                pass

            elif parser.current_instruction_type is instruction.L_INSTRUCTION:
                pass

    filename = "output.txt"

    with open(filename, "w") as f:
        f.write("\n".join(converted_instructions) + "\n")

    input("\nPress Enter to exit...")
