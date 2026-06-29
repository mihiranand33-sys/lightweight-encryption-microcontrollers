# Lightweight Encryption Algorithms for Embedded Microcontrollers

Overview
This repository contains implementations of lightweight encryption algorithms written in C for embedded microcontrollers. The objective is to study their implementation, memory usage, execution time, and suitability for resource-constrained embedded systems.

Algorithms implemented:

AES-128,
PRESENT-80

The implementations are intended for educational purposes and embedded security research.

Objectives:

1.Implement encryption algorithms in C.
2.Verify encryption and decryption.
3.Port the implementations to embedded hardware.
4.Measure Flash usage.
5.Measure RAM usage.
6.Measure execution latency.
7.Compare the algorithms.

*AES-128
Overview

AES (Advanced Encryption Standard) is a symmetric block cipher standardized by NIST.
It encrypts 128-bit plaintext using a 128-bit secret key in turn producing a 128-bit ciphertext.

AES consists of 10 rounds.
Each round performs

SubBytes
ShiftRows
MixColumns
AddRoundKey

The final round excludes MixColumns.

WORKING:
Key Expansion
Generates 11 round keys from the original 128-bit key.

AddRoundKey
Performs XOR between the current state and the round key.
state = state XOR roundKey

SubBytes
Each byte is replaced using the AES S-box.
Provides confusion.

ShiftRows
Rows are cyclically shifted left.
Provides diffusion.

MixColumns
Each column is multiplied in the finite field GF(2⁸).

