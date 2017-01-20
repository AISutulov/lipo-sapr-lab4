#include "yla_string.h"

/**
 * Char to integer convertation
 * @param {char} letter Letter
 * @return {int} Letter in integer convertation
 */
int char_to_int(char letter) 
{
	// Hexadecimal letter with \0 in the end
	// 'H' on char => 48 on int => {4, 8, \0} on char[]
	char hex_letter[2 + 1];
	
	sprintf(hex_letter, "%d", letter);
	return atoi(hex_letter);
}

/**
 * Put string repeater
 */
void put_string_repeat(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[], yla_int_type count_word_length_addr)
{
	yla_int_type prog_count = *prog_counter;
	
	yla_int_type word_length = global_var[0];
	yla_int_type repeat = global_var[1];
	yla_int_type i = global_var[2];
	yla_int_type ret = global_var[3];
	
	// Set subprogram starting address
	compliance_table_set_addr(compliance, subprog_start_addr, prog_count);
	
	// Start subprogram
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	
	// Local vars section
	// repeat = repeats count
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, repeat);					prog_count += 2;
	// i = 0 (iterator of word letters)
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0000);					prog_count += 2;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	// word_length = goto count_word_length
	put_commd(prog_ptr, CALO);						prog_count++;
	put_value(prog_ptr, count_word_length_addr);	prog_count += 2;
	// End local vars section
	
	// while (repeat != 0)
	/*start_repeat(511)*/
	compliance_table_set_addr(compliance, 0x0511, prog_count);
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, repeat);					prog_count += 2;
	
	put_commd(prog_ptr, CJZ);						prog_count++;
	put_value(prog_ptr, 0x0519);					prog_count += 2;	/*ret_subprogram(519)*/
	
	// {
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	// for (i = 0; i < word_length; i++)
	// (i = 0)
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0000);					prog_count += 2;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	/*start_word_letters(512)*/
	compliance_table_set_addr(compliance, 0x0512, prog_count);
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	put_commd(prog_ptr, CCMP);						prog_count++;
	// {
	put_commd(prog_ptr, CJNZ);						prog_count++;
	put_value(prog_ptr, 0x0513);					prog_count += 2;	/*end_word_letters(513)*/
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	// CGDUP word_length-1:
	// a. (word_length - 1)
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CSUB);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	// b. CGDUP word_length
	put_commd(prog_ptr, CGDUP);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	// c. (word_length + 1)
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CADD);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	// (i++)
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CADD);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, i);							prog_count += 2;
	
	// }
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0512);					prog_count += 2;
	
	/*end_word_letters(513)*/
	compliance_table_set_addr(compliance, 0x0513, prog_count);
	// Delete result of comparation
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	// repeat--
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, repeat);					prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CSUB);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, repeat);					prog_count += 2;
	
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0511);					prog_count += 2;
	
	/*ret_subprogram(519)*/
	compliance_table_set_addr(compliance, 0x0519, prog_count);
	
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	put_commd(prog_ptr, CALO);						prog_count++;
	put_value(prog_ptr, count_word_length_addr);	prog_count += 2;
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	put_commd(prog_ptr, CRET);						prog_count++;
	
	*prog_counter = prog_count;
}

/**
 * Put string concatenator subprogram
 */
void put_string_concat(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[], yla_int_type count_word_length_addr)
{
	yla_int_type prog_count = *prog_counter;
	
	yla_int_type word_length = global_var[0];
	yla_int_type ret = global_var[3];
	
	// Set subprogram starting address
	compliance_table_set_addr(compliance, subprog_start_addr, prog_count);
	
	// Start subprogram
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	
	// Local vars section
	// word_length = goto count_word_length
	put_commd(prog_ptr, CALO);						prog_count++;
	put_value(prog_ptr, count_word_length_addr);	prog_count += 2;
	// End local vars section
	
	// while (word_length != 0)
	compliance_table_set_addr(compliance, 0x0521, prog_count);	/*move_letter(521)*/
	
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	put_commd(prog_ptr, CJZ);						prog_count++;
	put_value(prog_ptr, 0x0522);					prog_count += 2;	/*end_subprogram(522)*/
	// {
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	// CGDUP word_length - 1:
	// a. (word_length - 1)
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CSUB);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	// b. (CGDUP word_length)
	put_commd(prog_ptr, CGDUP);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	// CGDEEP word_length:
	// a. (word_length + 1)
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CADD);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	// b. (CGDEEP word_length)
	put_commd(prog_ptr, CGDEEP);					prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	// word_length--
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CSUB);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	// }
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0521);					prog_count += 2;	/*move_letter(521)*/
	
	/*end_subprogram(522)*/
	compliance_table_set_addr(compliance, 0x0522, prog_count);	/*end_subprogram(521)*/
	
	// Delete last letter from stack
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	/*ret_subprogram(529)*/
	compliance_table_set_addr(compliance, 0x0529, prog_count);	/*ret_subprogram(529)*/
	// Delete result of comparation
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	// Count new word length
	put_commd(prog_ptr, CALO);						prog_count++;
	put_value(prog_ptr, count_word_length_addr);	prog_count += 2;
	// Ret subprogram
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	put_commd(prog_ptr, CRET);						prog_count++;
	
	*prog_counter = prog_count;
}

/**
 * Put count word length subprogram
 */
void put_count_word_length(yla_int_type **prog_ptr, yla_int_type subprog_start_addr, compliance_table *compliance, yla_int_type *prog_counter, yla_int_type global_var[])
{
	yla_int_type prog_count = *prog_counter;
	
	yla_int_type word_length = global_var[0];
	yla_int_type ret = global_var[4];
	
	// Set subprogram starting address
	compliance_table_set_addr(compliance, subprog_start_addr, prog_count);
	
	// Local vars section
	// word_length = 0
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0000);					prog_count += 2;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	// End local vars section
	
	// Start subprogram 
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, ret);						prog_count += 2;
	
	// 0. if (stack.top == TERMINATOR)
	// (i.e. word is empty)
	put_commd(prog_ptr, CDUP);						prog_count++;
	put_value(prog_ptr, 0x0000);					prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, YLA_WORD_NULL_TERMINATOR);	prog_count += 2;
	put_commd(prog_ptr, CCMP);						prog_count++;
	
	// { CRET
	put_commd(prog_ptr, CJNZ);						prog_count++;
	put_value(prog_ptr, 0x0509);					prog_count += 2;	/*ret_subprogram(509)*/
	
	// }
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	/*dup_letter(501)*/
	compliance_table_set_addr(compliance, 0x0501, prog_count);
	
	// if (stack.top == TERMINATOR)
	// Get letter from deep of stack
	put_commd(prog_ptr, CGDUP);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, YLA_WORD_NULL_TERMINATOR);	prog_count += 2;
	put_commd(prog_ptr, CCMP);						prog_count++;
	
	// {
	put_commd(prog_ptr, CJZ);						prog_count++;
	put_value(prog_ptr, 0x0502);					prog_count += 2;	/*increment_length(502)*/
	
	// Ret subprogram
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0509);					prog_count += 2;	/*ret_subprogram(509)*/
	
	/*increment_length(502)*/
	compliance_table_set_addr(compliance, 0x0502, prog_count);
	
	// } else {
	// Delete symbol from top of stack
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	
	// word_length++
	put_commd(prog_ptr, CLOAD);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	put_commd(prog_ptr, CPUSH);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;
	put_commd(prog_ptr, CADD);						prog_count++;
	put_commd(prog_ptr, CSAVE);						prog_count++;
	put_value(prog_ptr, word_length);				prog_count += 2;
	
	// goto dup_letter(501)
	put_commd(prog_ptr, CJMP);						prog_count++;
	put_value(prog_ptr, 0x0501);					prog_count += 2;	/*dup_letter(501)*/
	
	/*ret_subprogram(509)*/
	compliance_table_set_addr(compliance, 0x0509, prog_count);
	
	// Delete result of comparation
	put_commd(prog_ptr, CSTK);						prog_count++;
	put_value(prog_ptr, 0x0001);					prog_count += 2;	
	// Ret subprogram
	put_commd(prog_ptr, CLOAD);						prog_count++;	
	put_value(prog_ptr, ret);						prog_count += 2;	
	put_commd(prog_ptr, CRET);						prog_count++;	
	
	*prog_counter = prog_count;
}