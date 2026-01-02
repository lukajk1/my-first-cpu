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
        self.has_more_lines = len(self.instructions) > 0
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
        if self.current_inst_index >= len(self.instructions):
            self.has_more_lines = False
            return

        self.current_instruction = self.instructions[self.current_inst_index]
        # startswith() is safer than accessing by index as if the string is empty it can crash
        if self.current_instruction.startswith("@"):
            self.current_instruction_type = instruction.A_INSTRUCTION
        elif "=" in self.current_instruction or ";" in self.current_instruction:
            self.current_instruction_type = instruction.C_INSTRUCTION
        elif self.current_instruction.startswith("("):
            self.current_instruction_type = instruction.L_INSTRUCTION

        self.current_inst_index += 1

    def symbol(self):
        """Returns the symbol for A-instruction or label"""
        if self.current_instruction_type == instruction.A_INSTRUCTION:
            return self.current_instruction[1:]  # Remove @
        elif self.current_instruction_type == instruction.L_INSTRUCTION:
            return self.current_instruction[1:-1]  # Remove ( and )
        return ""

    def dest(self):
        """Returns dest part of C-instruction"""
        if '=' in self.current_instruction:
            return self.current_instruction.split('=')[0]
        return ""

    def comp(self):
        """Returns comp part of C-instruction"""
        inst = self.current_instruction
        # Remove dest if present
        if '=' in inst:
            inst = inst.split('=')[1]
        # Remove jump if present
        if ';' in inst:
            inst = inst.split(';')[0]
        return inst

    def jump(self):
        """Returns jump part of C-instruction"""
        if ';' in self.current_instruction:
            return self.current_instruction.split(';')[1]
        return ""

class Code:
    @staticmethod
    def dest(mnemonic):
        """Returns 3-bit dest code"""
        d1 = '1' if 'A' in mnemonic else '0'
        d2 = '1' if 'D' in mnemonic else '0'
        d3 = '1' if 'M' in mnemonic else '0'
        return d1 + d2 + d3

    @staticmethod
    def comp(mnemonic):
        """Returns 7-bit comp code"""
        comp_table = {
            # a=0 computations
            '0':   '0101010',
            '1':   '0111111',
            '-1':  '0111010',
            'D':   '0010011',
            'A':   '0010010',
            '!D':  '0001101',
            '!A':  '0110001',
            '-D':  '0001111',
            '-A':  '0110011',
            'D+1': '0011111',
            'A+1': '0110111',
            'D-1': '0001110',
            'A-1': '0110010',
            'D+A': '0000010',
            'D-A': '0010011',
            'A-D': '0000111',
            'D&A': '0000000',
            'D|A': '0010101',
            # a=1 computations (using M instead of A)
            'M':   '1110000',
            '!M':  '1110001',
            '-M':  '1110011',
            'M+1': '1110111',
            'M-1': '1110010',
            'D+M': '1000010',
            'D-M': '1010011',
            'M-D': '1000111',
            'D&M': '1000000',
            'D|M': '1010101',
        }
        return comp_table.get(mnemonic, '0000000')

    @staticmethod
    def jump(mnemonic):
        """Returns 3-bit jump code"""
        jump_table = {
            '':    '000',
            'JGT': '001',
            'JEQ': '010',
            'JGE': '011',
            'JLT': '100',
            'JNE': '101',
            'JLE': '110',
            'JMP': '111',
        }
        return jump_table.get(mnemonic, '000')

class SymbolTable:
    def __init__(self):
        self.table = {
            'SP': 0, 'LCL': 1, 'ARG': 2, 'THIS': 3, 'THAT': 4,
            'R0': 0, 'R1': 1, 'R2': 2, 'R3': 3, 'R4': 4,
            'R5': 5, 'R6': 6, 'R7': 7, 'R8': 8, 'R9': 9,
            'R10': 10, 'R11': 11, 'R12': 12, 'R13': 13,
            'R14': 14, 'R15': 15,
            'SCREEN': 16384, 'KBD': 24576
        }
        self.next_var_address = 16

    def add_entry(self, symbol, address):
        self.table[symbol] = address

    def contains(self, symbol):
        return symbol in self.table

    def get_address(self, symbol):
        # If it's a number, return it as int
        if symbol.isdigit():
            return int(symbol)

        # If it's a symbol not in table, add as variable
        if not self.contains(symbol):
            self.add_entry(symbol, self.next_var_address)
            self.next_var_address += 1

        return self.table[symbol]

# D=A+1;JEQ
# comp = A+1, dest = D, jump = JEQ

if __name__ == "__main__":
    print("=== Hack Assembler ===")
    print("Enter 'q' to quit\n")

    while True:
        filename = input("Enter filename of .asm file in /programs: ").strip()

        # Check for quit command
        if filename.lower() == 'q':
            print("Exiting assembler...")
            break

        # Add .asm extension if not specified
        if not filename.endswith('.asm'):
            filename += '.asm'

        current_dir = os.path.dirname(os.path.abspath(__file__))
        target_path = os.path.join(current_dir, "programs", filename)

        if not os.path.exists(target_path):
            print(f"Error: Could not find {filename} in 'programs' directory.")
            print()
            continue

        print(f"File found: {target_path}")

        try:
            # First pass: build symbol table with labels
            parser = Parser(target_path)
            symbol_table = SymbolTable()

            instruction_address = 0
            while parser.has_more_lines:
                parser.advance()

                if parser.current_instruction_type == instruction.L_INSTRUCTION:
                    # Add label to symbol table
                    symbol = parser.symbol()
                    symbol_table.add_entry(symbol, instruction_address)
                else:
                    # Count actual instructions
                    instruction_address += 1

            # Second pass: generate machine code
            parser = Parser(target_path)
            code = Code()
            converted_instructions = []

            while parser.has_more_lines:
                parser.advance()

                if parser.current_instruction_type == instruction.A_INSTRUCTION:
                    symbol = parser.symbol()
                    address = symbol_table.get_address(symbol)
                    # A-instruction: 0 + 15-bit address
                    binary = f"0{address:015b}"
                    converted_instructions.append(binary)

                elif parser.current_instruction_type == instruction.C_INSTRUCTION:
                    dest_bits = code.dest(parser.dest())
                    comp_bits = code.comp(parser.comp())
                    jump_bits = code.jump(parser.jump())
                    # C-instruction: 111 + comp + dest + jump
                    binary = f"111{comp_bits}{dest_bits}{jump_bits}"
                    converted_instructions.append(binary)

                elif parser.current_instruction_type == instruction.L_INSTRUCTION:
                    # Labels don't generate machine code
                    pass

            # Write output to programs directory with same name as input
            base_filename = os.path.splitext(filename)[0]  # Remove .asm extension
            output_filename = base_filename + ".hack"
            output_path = os.path.join(current_dir, "programs", output_filename)

            with open(output_path, "w") as f:
                f.write("\n".join(converted_instructions) + "\n")

            print(f"\n=== Assembly Complete ===")
            print(f"Instructions assembled: {len(converted_instructions)}")
            print(f"Output written to: programs/{output_filename}")
            print()

            # Show first few lines
            print("First 10 instructions:")
            for i, inst in enumerate(converted_instructions[:10]):
                print(f"{i:3d}: {inst}")
            print()

        except Exception as e:
            print(f"Error during assembly: {e}")
            print()
