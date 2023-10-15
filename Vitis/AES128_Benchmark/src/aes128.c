/********************************************************************************
* @file     aes128.c                                                            *
* @brief    AES128 Implementation File                                          *
* @author   Yeshvanth M  <yeshvanthmuniraj@gmail.com>                           *
* @date     14-Oct-2023                                                         *
*********************************************************************************
*                                                                               *
* This program is free software: you can redistribute it and/or modify it       *
* under the terms of the GNU General Public License as published by the Free    *
* Software Foundation, either version 3 of the License, or (at your option)     *
* any later version.                                                            *
*                                                                               *
* This program is distributed in the hope that it will be useful, but           *
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY    *
* or FITNESS FOR A PARTICULAR PURPOSE.                                          *
* See the GNU General Public License for more details.                          *
*                                                                               *
* You should have received a copy of the GNU General Public License             *
* along with this program. If not, see <https://www.gnu.org/licenses/>.         *
*                                                                               *
********************************************************************************/

#include "aes128.h"

/* The state used by the AES engine */
uint8_t aes128_state[16u] =
{
    0x4C, 0x6D, 0x73, 0x64,
    0x6F, 0x20, 0x75, 0x6F,
    0x72, 0x69, 0x6D, 0x6C,
    0x65, 0x70, 0x20, 0x6F
};

/* The cipher key used by the AES engine */
uint8_t aes128_cipherKey[16u] =
{
    0x2B, 0x28, 0xAB, 0x09,
    0x7E, 0xAE, 0xF7, 0xCF,
    0x15, 0xD2, 0x15, 0x4F,
    0x16, 0xA6, 0x88, 0x3C
};

/* Add round key to the state */
void aes128_add_round_key (uint8_t round, bool aes128_is_encrypt)
{
    uint8_t *round_key;

    if (aes128_is_encrypt)
    {
        if (round == 0)
        {
        	/* Use initial cipher key in round 0 for encryption */
            round_key = aes128_cipherKey;
        }
        else
        {
        	/* Use keys generated in key schedule for consecutive rounds */
            round--;
            round_key = aes128_round_keys[round];
        }
    }
    else
    {
    	/* Last generated key is used in the first round for decryption */
        round = 10 - round;
        if (round == 0)
        {
        	/* Use initial cipher key in round 10 */
            round_key = aes128_cipherKey;
        }
        else
        {
        	/* Use keys generated in key schedule for other rounds */
            round--;
            round_key = aes128_round_keys[round];
        }
    }

    for (uint8_t idx = 0; idx < 16u; idx++)
    {
    	/* Add key to the state */
        aes128_state[idx] ^= round_key[idx];
    }

}

void aes128_substitute_bytes (bool aes128_is_encrypt)
{
    const uint8_t *sBoxMat;

    /* Use SBox for encryption and inverse SBox for decryption */
    if (aes128_is_encrypt)
    {
        sBoxMat = aes128_sBox;
    }
    else
    {
        sBoxMat = aes128_sBox_Inv;
    }

    for (uint8_t idx = 0u; idx < 16u; idx++)
    {
    	/* Substitute bytes */
        aes128_state[idx] = sBoxMat[aes128_state[idx]];
    }
}

void aes128_shift_rows (bool aes128_is_encrypt)
{
    if (aes128_is_encrypt)
    {
    	/* Circular left shift for encryption */
        for (uint8_t row = 1u; row < 4u; row++)
        {
            uint8_t shift = row;
            while (shift > 0u)
            {
                uint8_t col = 0u, temp = aes128_state[(row * STATE_ROWS) + col];
                while (col < 3u)
                {
                    aes128_state[(row * STATE_ROWS) + col] = aes128_state[(row * STATE_ROWS) + col + 1u];
                    col++;
                }
                aes128_state[(row * STATE_ROWS) + col] = temp;
                shift--;
            }
        }
    }
    else
    {
    	/* Circular right shift for decryption */
        for (uint8_t row = 1u; row < 4u; row++)
        {
            uint8_t shift = row;
            while (shift > 0u)
            {
                uint8_t col = 3u, temp = aes128_state[(row * STATE_ROWS) + col];
                while (col > 0u)
                {
                    aes128_state[(row * STATE_ROWS) + col] = aes128_state[(row * STATE_ROWS) + col - 1u];
                    col--;
                }
                aes128_state[(row * STATE_ROWS) + col] = temp;
                shift--;
            }
        }
    }
}

void aes128_mix_columns (bool aes128_is_encrypt)
{
    uint8_t *column_mix_matrix;

    /* Switch between matrices for multiplication for encrypt and decrypt */
    if (aes128_is_encrypt)
    {
        column_mix_matrix = aes128_column_mix_matrix;
    }
    else
    {
        column_mix_matrix = aes128_column_mix_matrix_inv;
    }

    for (uint8_t state_col = 0u; state_col < 4u; state_col++)
    {
        uint8_t state_column[4u];
        for (uint8_t matrix_row = 0u; matrix_row < 4u; matrix_row++)
        {
            uint8_t col_sum = 0u;
            for (uint8_t matrix_col = 0u; matrix_col < 4u; matrix_col++)
            {
                switch (column_mix_matrix[(matrix_row * COL_MAT_ROWS) + matrix_col])
                {
                    case 0x1:
                    {
                        col_sum ^= aes128_state[(matrix_col * STATE_ROWS) + state_col];
                        break;
                    }
                    case 0x2:
                    {
                        col_sum ^= aes128_mul_by_2[aes128_state[(matrix_col * STATE_ROWS) + state_col]];
                        break;
                    }
                    case 0x3:
                    {
                        col_sum ^= aes128_mul_by_3[aes128_state[(matrix_col * STATE_ROWS) + state_col]];
                        break;
                    }
                    case 0x9:
                    {
                        col_sum ^= aes128_mul_by_9[aes128_state[(matrix_col * STATE_ROWS) + state_col]];
                        break;
                    }
                    case 0xb:
                    {
                        col_sum ^= aes128_mul_by_b[aes128_state[(matrix_col * STATE_ROWS) + state_col]];
                        break;
                    }
                    case 0xd:
                    {
                        col_sum ^= aes128_mul_by_d[aes128_state[(matrix_col * STATE_ROWS) + state_col]];
                        break;
                    }
                    case 0xe:
                    {
                        col_sum ^= aes128_mul_by_e[aes128_state[(matrix_col * STATE_ROWS) + state_col]];
                        break;
                    }
                    default:
                        break;
                }
            }
            state_column[matrix_row] = col_sum;
        }
        for (uint8_t state_row = 0; state_row < 4; state_row++)
        {
            aes128_state[(state_row * STATE_ROWS) + state_col] = state_column[state_row];
        }
    }
}

void aes128_key_schedule (void)
{
	/* Generate 10 keys for 10 rounds */
    memcpy ((void *)aes128_round_keys[0u], (void *)aes128_cipherKey, sizeof(aes128_cipherKey));
    for (uint8_t round = 0; round < 10; round++)
    {
    	/* Form the RCON matrix */
        uint8_t rcon[4u] = {aes128_rcon[round], 0u, 0u, 0u};
        uint8_t round_key_col[4u];

        uint8_t row = 0u, col = 3u, temp = aes128_round_keys[round][(row * KEY_ROWS) + col];
        while (row < 3u)
        {
        	/* Get the last column */
            round_key_col[row] = aes128_round_keys[round][((row + 1u) * KEY_ROWS) + col];
            row++;
        }
        round_key_col[row] = temp;

        for (row = 0u; row < 4u; row++)
        {
        	/* Substitute bytes */
            round_key_col[row] = aes128_sBox[round_key_col[row]];
        }

        for (col = 0u; col < 4u; col++)
        {
            if (col == 0u)
            {
                for (row = 0u; row < 4u; row++)
                {
                	/* Multiply with Rcon and column of previous key */
                    aes128_round_keys[round][(row * KEY_ROWS) + col] ^= (rcon[row] ^ round_key_col[row]);
                }
            }
            else
            {
                for (row = 0u; row < 4u; row++)
                {
                	/* Multiply with column of previous key */
                    aes128_round_keys[round][(row * KEY_ROWS) + col] ^= aes128_round_keys[round][(row * KEY_ROWS) + col - 1u];
                }
            }
        }
        /* Save the generated key */
        memcpy ((void *)aes128_round_keys[round + 1], (void *)aes128_round_keys[round], sizeof(aes128_round_keys[round]));
    }
}

void aes128_gen_keys(void)
{
    aes128_key_schedule ();
}

void aes128_encrypt(void)
{
	uint8_t round = 0u;
	bool encrypt = true;

	/* Add round key and perform 9 rounds */
    aes128_add_round_key (round, encrypt);
    for (round = 1; round < 10; round++)
    {
        aes128_substitute_bytes (encrypt);
        aes128_shift_rows (encrypt);
        aes128_mix_columns (encrypt);
        aes128_add_round_key (round, encrypt);
    }
    /* Final round */
    aes128_substitute_bytes (encrypt);
    aes128_shift_rows (encrypt);
    aes128_add_round_key (round, encrypt);
}

void aes128_decrypt(void)
{
	uint8_t round = 0u;
	bool decrypt = false;

	/* Add round key and perform 9 rounds*/
    aes128_add_round_key (round, decrypt);
    for (round = 1; round < 10; round++)
    {
        aes128_shift_rows (decrypt);
        aes128_substitute_bytes (decrypt);
        aes128_add_round_key (round, decrypt);
        aes128_mix_columns (decrypt);
    }
    /* Final round */
    aes128_shift_rows (decrypt);
    aes128_substitute_bytes (decrypt);
    aes128_add_round_key (round, decrypt);
}
