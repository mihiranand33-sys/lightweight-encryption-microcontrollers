#include <stdio.h>
#include <stdint.h>

uint8_t Sbox[16] = {
    0xC, 0x5, 0x6, 0xB,
    0x9, 0x0, 0xA, 0xD,
    0x3, 0xE, 0xF, 0x8,
    0x4, 0x7, 0x1, 0x2
};

uint8_t InvSbox[16] = {
    0x5, 0xE, 0xF, 0x8,
    0xC, 0x1, 0x2, 0xD,
    0xB, 0x4, 0x6, 0x3,
    0x0, 0x7, 0x9, 0xA
};

void InvSboxfn(uint64_t *state){
    int i;
    uint64_t temp = 0;
    uint8_t nibble;
    for(i = 0; i < 16; i++){
        nibble=(*state>>(i*4)) & 0xF; 
        nibble = InvSbox[nibble];
        temp |= ((uint64_t)nibble << (i*4));
    }
    *state = temp;
}
void InvPLayer(uint64_t *state)
{
    uint64_t temp = 0;
    uint64_t bit;
    int i;
    int np;

    for(i = 0; i < 63; i++){
        bit = (*state >> i) & 1;
        np = (i * 4) % 63;
        temp |= (bit << np);
        
    
    }
     bit = (*state >> 63) & 1;
    temp |= bit << 63;

    *state = temp;
}

void AddRK(uint64_t *state, uint64_t roundKey)
{
    *state ^= roundKey;
}


void Decrypt(uint64_t *state,
             uint64_t roundKeys[])
{
    int round;

    AddRK(state, roundKeys[31]);

    for(round = 30; round >= 0; round--)
    {
        InvPLayer(state);
        InvSboxfn(state);
        AddRK(state, roundKeys[round]);
    }
}

void Sboxfn(uint64_t *state)
{
    int i;
    uint64_t temp = 0;
    uint8_t nibble;
    for(i = 0; i < 16; i++){
        nibble=(*state>>(i*4)) & 0xF; 
        nibble = Sbox[nibble];
        temp |= ((uint64_t)nibble << (i*4));
    }
    *state = temp;
}



void PLayer(uint64_t *state)
{
    uint64_t temp = 0;
    uint64_t bit;
    int i;
    int np;

    for(i = 0; i < 63; i++)
    {
        // Extract bit i from the state
        bit = (*state >> i) & 1;
        // Calculate the new position for this bit
        np = (i * 16) % 63;
        // Place the bit in its new position
        temp |= (bit << np);


}
 // Bit 63 stays at position 63
    bit = (*state >> 63) & 1;
    temp |= bit << 63;

    // Update the state
    *state = temp;
}



void KeySchedule(uint64_t *key_upper, uint16_t *key_lower, int round)
{
    uint64_t temp_upper;
    uint16_t temp_lower;

    temp_upper = *key_upper;
    temp_lower = *key_lower;
 // Rotate the 80-bit key left by 61 bits
    *key_upper = (temp_upper << 61)
               | ((uint64_t)temp_lower << 45)
               | (temp_upper >> 19);
    *key_lower = (temp_upper >> 3) & 0xFFFF;    
    
    //to store the leftmost 4 bits of the key.
    uint8_t sbox_ip;
    //to store the substituted value.
    uint8_t sbox_op;


    sbox_ip= (*key_upper >> 60) & 0xF;
    sbox_op=Sbox[sbox_ip];
    *key_upper &= 0x0FFFFFFFFFFFFFFF;
*key_upper |= ((uint64_t)sbox_op << 60);

// XOR round bits 4-1 into key bits 19-16
*key_upper ^= ((uint64_t)((round >> 1) & 0xF));

// XOR round bit 0 into key bit 15
*key_lower ^= ((round & 0x1) << 15);

}

void Encrypt(uint64_t *state,
             uint64_t *key_upper,
             uint16_t *key_lower,
             uint64_t roundKeys[])
{
    int round;

    // Store initial round key
    roundKeys[0] = *key_upper;

    for(round = 1; round <= 31; round++)
    {
        AddRK(state, *key_upper);
        Sboxfn(state);
        PLayer(state);

        KeySchedule(key_upper, key_lower, round);

        // Store generated round key
        roundKeys[round] = *key_upper;
    }

    // Final whitening key
    AddRK(state, *key_upper);
}

int main()
{
    int i;
    printf("S box for PRESENT:\n");
    for (i = 0; i < 16; i++)
    {
        printf("%X is %X\n", i, Sbox[i]);
    }

   
    uint64_t state = 0x0000000000000000;
uint64_t key_upper = 0x0000000000000000;
uint16_t key_lower = 0x0000;
uint64_t roundKeys[32];

    Encrypt(&state, &key_upper, &key_lower, roundKeys);

    printf("Encrypted text = %016llX\n", (unsigned long long)state);

    Decrypt(&state, roundKeys);

    printf("Plaintext = %016llX\n",(unsigned long long)state);

    return 0;
}