-------------------------------------------------------------------------------
-- File name    :  $Id: 80C51CPUSIM.vhd,v 1.4 2003/06/12 09:09:07 frha Exp $
-- ----------------------------------------------------------------------------
-- Revision History :
-- ----------------------------------------------------------------------------
-- $Log: 80C51CPUSIM.vhd,v $
-- Revision 1.4  2003/06/12 09:09:07  frha
-- Added revision string
--

library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.ALL;
use std.textio.all;

package CPU_PACK is
    type INST_TYPE is (READ, WRITE, NONE);
    type SHOW_TYPE is (READ0, READ1, WRITE0, WRITE1, VERIFY0, VERIFY1, LOOP0, LOOP1, NONE, END0);

    function EqualSLV(A : std_logic_vector; B : std_logic_vector) return boolean;

    procedure ReadHex(L:inout line; IVAL: out unsigned);
    procedure ReadExtra(L:inout line; IVAL: out unsigned);
	
    procedure WriteHex(L:inout line; OVAL: in unsigned);
end CPU_PACK;

package body CPU_PACK is

    function EqualSLV(A : std_logic_vector; B : std_logic_vector) return boolean is
	    variable I : integer;
	begin
	    for I in 0 to 7 loop
		    if ((A(I) /= '-') and (B(I) /= '-') and (A(I) /= B(I))) then
                return false;
			end if;
		end loop;
		return true;
	end function;



    procedure ReadHex(L:inout line; IVAL: out unsigned) is
	    variable C : character;
	    variable TMP : unsigned(31 downto 0) := (others => '0');
		variable DONE : boolean;
	begin
        read(L, C);
		while ((C = HT) or (C = ' ')) loop
            read(L, C);
		end loop;
		DONE := false;
        while (DONE = false) loop

			case C is
			when 'a' =>
				C := 'A';
			when 'b' =>
				C := 'B';
			when 'c' =>
				C := 'C';
			when 'd' =>
				C := 'D';
			when 'e' =>
				C := 'E';
			when 'f' =>
				C := 'F';
			when others =>
				null;
			end case;

    		case C is
    		when '0' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(0, 4);
                read(L, C);
    		when '1' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(1, 4);
                read(L, C);
    		when '2' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(2, 4);
                read(L, C);
    		when '3' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(3, 4);
                read(L, C);
    		when '4' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(4, 4);
                read(L, C);
    		when '5' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(5, 4);
                read(L, C);
    		when '6' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(6, 4);
                read(L, C);
    		when '7' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(7, 4);
                read(L, C);
    		when '8' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(8, 4);
                read(L, C);
    		when '9' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(9, 4);
                read(L, C);
    		when 'A' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(10, 4);
                read(L, C);
    		when 'B' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(11, 4);
                read(L, C);
    		when 'C' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(12, 4);
                read(L, C);
    		when 'D' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(13, 4);
                read(L, C);
    		when 'E' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(14, 4);
                read(L, C);
    		when 'F' =>
    		    TMP := TMP(27 downto 0) & to_unsigned(15, 4);
                read(L, C);
    		when others =>
                IVAL := TMP;
				DONE := true;
    		end case;
		end loop;
	end ReadHex;




    procedure ReadExtra(L:inout line; IVAL: out unsigned) is
	    variable C : character;
	    variable TMP : unsigned(31 downto 0) := (others => '0');
		variable DONE : boolean;
	begin
        read(L, C);
		while ((C = HT) or (C = ' ')) loop
            read(L, C);
		end loop;
		case C is
		when 'b' =>
			C := 'B';
		when 'x' =>
			C := 'X';
		when 'd' =>
			C := 'D';
		when others =>
			null;
		end case;

		case C is
		when 'B' => 
			DONE := false;
            read(L, C);
	        while (DONE = false) loop
	    		case C is
	    		when '0' =>
	    		    TMP := TMP(30 downto 0) & '0';
	                read(L, C);
	    		when '1' =>
	    		    TMP := TMP(30 downto 0) & '1';
	                read(L, C);
	    		when '-' =>
	    		    TMP := TMP(30 downto 0) & '-';
	                read(L, C);
	    		when others =>
					DONE := true;
	    		end case;
			end loop;			
		when 'X' =>
			DONE := false;
            read(L, C);
	        while (DONE = false) loop
				case C is
				when 'a' =>
					C := 'A';
				when 'b' =>
					C := 'B';
				when 'c' =>
					C := 'C';
				when 'd' =>
					C := 'D';
				when 'e' =>
					C := 'E';
				when 'f' =>
					C := 'F';
				when others =>
					null;
				end case;

	    		case C is
	    		when '0' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(0, 4);
	                read(L, C);
	    		when '1' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(1, 4);
	                read(L, C);
	    		when '2' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(2, 4);
	                read(L, C);
	    		when '3' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(3, 4);
	                read(L, C);
	    		when '4' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(4, 4);
	                read(L, C);
	    		when '5' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(5, 4);
	                read(L, C);
	    		when '6' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(6, 4);
	                read(L, C);
	    		when '7' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(7, 4);
	                read(L, C);
	    		when '8' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(8, 4);
	                read(L, C);
	    		when '9' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(9, 4);
	                read(L, C);
	    		when 'A' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(10, 4);
	                read(L, C);
	    		when 'B' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(11, 4);
	                read(L, C);
	    		when 'C' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(12, 4);
	                read(L, C);
	    		when 'D' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(13, 4);
	                read(L, C);
	    		when 'E' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(14, 4);
	                read(L, C);
	    		when 'F' =>
	    		    TMP := TMP(27 downto 0) & to_unsigned(15, 4);
	                read(L, C);
	    		when '-' =>
	    		    TMP := TMP(27 downto 0) & "----";
	                read(L, C);
	    		when others =>
					DONE := true;
	    		end case;
			end loop;
		when 'D' =>
			DONE := false;
            read(L, C);
	        while (DONE = false) loop
	    		case C is
	    		when '0' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(0, 4), 32);
	                read(L, C);
	    		when '1' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(1, 4), 32);
	                read(L, C);
	    		when '2' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(2, 4), 32);
	                read(L, C);
	    		when '3' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(3, 4), 32);
	                read(L, C);
	    		when '4' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(4, 4), 32);
	                read(L, C);
	    		when '5' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(5, 4), 32);
	                read(L, C);
	    		when '6' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(6, 4), 32);
	                read(L, C);
	    		when '7' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(7, 4), 32);
	                read(L, C);
	    		when '8' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(8, 4), 32);
	                read(L, C);
	    		when '9' =>
	    		    TMP := resize((TMP * 10) + to_unsigned(9, 4), 32);
	                read(L, C);
	    		when others =>
					DONE := true;
	    		end case;
			end loop;
			
		when others => 
			null;
		end case;
		IVAL := TMP;
	end ReadExtra;
	

    procedure WriteHex(L:inout line; OVAL: in unsigned) is
	    constant C_ZERO : character := '0';
	    constant C_ONE : character := '1';
		variable INDEX : integer;
		variable NIBBLES : integer;
		variable TMP : std_logic_vector(3 downto 0);
	begin
        NIBBLES := (OVAL'length / 4) - 1;

	    for INDEX in NIBBLES downto 0 loop
		    TMP := std_logic_vector(OVAL(4*INDEX+3 downto 4*INDEX));
			case TMP is
			when "0000" =>
			    write(L, C_ZERO);
			when "0001" =>
			    write(L, C_ONE);
			when "0010" =>
			    write(L, '2');
			when "0011" =>
			    write(L, '3');
			when "0100" =>
			    write(L, '4');
			when "0101" =>
			    write(L, '5');
			when "0110" =>
			    write(L, '6');
			when "0111" =>
			    write(L, '7');
			when "1000" =>
			    write(L, '8');
			when "1001" =>
			    write(L, '9');
			when "1010" =>
			    write(L, 'A');
			when "1011" =>
			    write(L, 'B');
			when "1100" =>
			    write(L, 'C');
			when "1101" =>
			    write(L, 'D');
			when "1110" =>
			    write(L, 'E');
			when "1111" =>
			    write(L, 'F');
			when "ZZZZ" =>
			    write(L, 'Z');
			when "XXXX" =>
			    write(L, 'X');
			when "WWWW" =>
			    write(L, 'W');
			when "LLLL" =>
			    write(L, 'L');
			when "HHHH" =>
			    write(L, 'H');
			when "UUUU" =>
			    write(L, 'U');
			when others =>
			    write(L, 'U');
			end case;
		end loop;
	end WriteHex;


end CPU_PACK;






library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.ALL;
use std.textio.all;
use work.CPU_PACK.all;

entity C51CPUSIM is
	generic (
		PW          : time := 20833 ps
	);
	port (
        RESET       : in std_logic;
	    ALE         : out std_logic;
		WR		    : out std_logic;
		RD	        : out std_logic;
		PORT0       : inout std_logic_vector(7 downto 0);
		PORT2		: out std_logic_vector(7 downto 0);
		PORT1		: out std_logic_vector(7 downto 0);
		IRQ_N       : in std_logic
	);
end entity C51CPUSIM;		     	

architecture MODEL of C51CPUSIM is

    constant s1 : string := "ADDRESS:        DATA:           TIME:";
    constant s2 : string := "Data verified correct at time: ";
    constant s3 : string := "Data verification error at time: ";
    constant s4 : string := "Exit loop at time: ";

	signal XTAL         : std_logic;
	signal XTAL_i       : std_logic;
--	signal CURR_INST    : SHOW_TYPE;

    file INPUT_FILE : text open read_mode is "CPU_CODE.TXT";
    file OUTPUT_FILE : text open write_mode is "CPU_LOG.TXT";
	
begin
	

    XTALPROCESS : process
    begin
        XTAL_i <= '0';
        wait for PW / 2;
        XTAL_i <= '1';
        wait for PW / 2;
    end process XTALPROCESS;
	
	XTAL <= XTAL_i;

    CPU : process
		variable OUTPUT_LINE    : line;
		variable INPUT_LINE     : line;
        variable C_CODE         : character;
		variable C_CHAR         : character;
		variable C_HEXSTR       : string(1 to 15);
		variable C_DELAY        : time;
		variable C_PORTBIT      : integer;
		variable C_BITVALUE     : integer;
		variable CONV           : unsigned(31 downto 0);
--    	variable LAST_INST      : INST_TYPE;
    	variable TMP_ADR        : unsigned(31 downto 0) ;
	    variable TMP_DATA       : std_logic_vector(31 downto 0) ;
		variable DONE           : boolean;
	begin
		TMP_ADR := (others => '0');
		TMP_DATA := (others => 'Z');
 		RD <= '1';
 		WR <= '1';
		ALE <= '0';
 		PORT0 <= (others => 'H');
		PORT1 <= (others => '0');
		PORT2 <= "HHHHHHHH";
        if RESET /= '0' then        
            wait until RESET = '0'; 
        end if;                     
	    wait until RESET = '1';     
 		wait until falling_edge(XTAL); 
		PORT0 <= (others => 'Z');
		RD <= '1';
		WR <= '1';
		ALE <= '0';

		write(OUTPUT_LINE, s1);
		writeline(OUTPUT_FILE, OUTPUT_LINE);

        while not endfile(INPUT_FILE) loop
            readline(INPUT_FILE, INPUT_LINE);
    		read(INPUT_LINE, C_CODE);
    		case (C_CODE) is
    		when 'H' => 
   		        read(INPUT_LINE, C_DELAY);
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				PORT0 <= (others => 'Z');
    			wait for C_DELAY;
			when 'P' =>
	   		    read(INPUT_LINE, C_PORTBIT);
	   		    read(INPUT_LINE, C_BITVALUE);
				if (C_BITVALUE = 1) then
					PORT1(C_PORTBIT) <= '1';
				else 
					PORT1(C_PORTBIT) <= '0';
				end if;
                wait for 10 ns;
			when 'R' =>
                ReadHex(INPUT_LINE, CONV);
                TMP_ADR := CONV;
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				ALE <= '1';
				PORT0 <= (others => 'Z');
				wait until rising_edge(XTAL);
				PORT0 <= std_logic_vector(TMP_ADR(7 downto 0));
				if (TMP_ADR(15 downto 8) /= "00000000") then
					PORT2 <= std_logic_vector(TMP_ADR(15 downto 8));
				else
					PORT2 <= (others => 'H');
				end if;
				wait until falling_edge(XTAL);
				ALE <= '0';
				wait until rising_edge(XTAL);
				PORT0 <= (others => 'Z');
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				RD <= '0';
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				RD <= '1';
				WriteHex(OUTPUT_LINE, TMP_ADR);
	    		write(OUTPUT_LINE, HT);
	    		write(OUTPUT_LINE, HT);
				WriteHex(OUTPUT_LINE, unsigned(PORT0));
	    		write(OUTPUT_LINE, HT);
        		write(OUTPUT_LINE, now);
	    		writeline(OUTPUT_FILE, OUTPUT_LINE);
		 		wait until falling_edge(XTAL);
			when 'W' =>
			   	ReadExtra(INPUT_LINE, CONV);
                TMP_DATA := std_logic_vector(CONV);
                ReadHex(INPUT_LINE, CONV);
                TMP_ADR := CONV;
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				ALE <= '1';
				PORT0 <= (others => 'Z');
				wait until rising_edge(XTAL);
				PORT0 <= std_logic_vector(TMP_ADR(7 downto 0));
				if (TMP_ADR(15 downto 8) /= "00000000") then
					PORT2 <= std_logic_vector(TMP_ADR(15 downto 8));
				else
					PORT2 <= (others => 'H');
				end if;
				wait until falling_edge(XTAL);
				ALE <= '0';
				wait until rising_edge(XTAL);
				PORT0 <= (others => 'Z');
				wait until falling_edge(XTAL);
				PORT0 <= TMP_DATA(7 downto 0);
				wait until rising_edge(XTAL);
				WR <= '0';
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				PORT0 <= TMP_DATA(7 downto 0);
				wait until rising_edge(XTAL);
				WR <= '1';
				WriteHex(OUTPUT_LINE, TMP_ADR);
	    		write(OUTPUT_LINE, HT);
	    		write(OUTPUT_LINE, HT);
				WriteHex(OUTPUT_LINE, unsigned(PORT0));
	    		write(OUTPUT_LINE, HT);
        		write(OUTPUT_LINE, now);
	    		writeline(OUTPUT_FILE, OUTPUT_LINE);
		 		wait until falling_edge(XTAL);
			when 'V' =>
                ReadExtra(INPUT_LINE, CONV);
                TMP_DATA := std_logic_vector(CONV);
                ReadHex(INPUT_LINE, CONV);
                TMP_ADR := CONV;
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				ALE <= '1';
				PORT0 <= (others => 'Z');
				wait until rising_edge(XTAL);
				PORT0 <= std_logic_vector(TMP_ADR(7 downto 0));
				if (TMP_ADR(15 downto 8) /= "00000000") then
					PORT2 <= std_logic_vector(TMP_ADR(15 downto 8));
				else
					PORT2 <= (others => 'H');
				end if;
				wait until falling_edge(XTAL);
				ALE <= '0';
				wait until rising_edge(XTAL);
				PORT0 <= (others => 'Z');
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				RD <= '0';
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				wait until falling_edge(XTAL);
				wait until rising_edge(XTAL);
				RD <= '1';
                if (EqualSLV(TMP_DATA(7 downto 0), PORT0)) then
            		write(OUTPUT_LINE, s2);
            		write(OUTPUT_LINE, now);
            		writeline(OUTPUT_FILE, OUTPUT_LINE);
				else
            		write(OUTPUT_LINE, s3);
            		write(OUTPUT_LINE, now);
            		writeline(OUTPUT_FILE, OUTPUT_LINE);
				end if;
				WriteHex(OUTPUT_LINE, TMP_ADR);
	    		write(OUTPUT_LINE, HT);
	    		write(OUTPUT_LINE, HT);
				WriteHex(OUTPUT_LINE, unsigned(PORT0));
	    		write(OUTPUT_LINE, HT);
        		write(OUTPUT_LINE, now);
   	    		writeline(OUTPUT_FILE, OUTPUT_LINE);
		 		wait until falling_edge(XTAL);
			when 'L' =>
                ReadExtra(INPUT_LINE, CONV);
                TMP_DATA := std_logic_vector(CONV);
                ReadHex(INPUT_LINE, CONV);
                TMP_ADR := CONV;
                DONE := false;
                while (DONE = false) loop
					wait until rising_edge(XTAL);
					wait until falling_edge(XTAL);
					ALE <= '1';
					PORT0 <= (others => 'Z');
					wait until rising_edge(XTAL);
					PORT0 <= std_logic_vector(TMP_ADR(7 downto 0));
					if (TMP_ADR(15 downto 8) /= "00000000") then
						PORT2 <= std_logic_vector(TMP_ADR(15 downto 8));
					else
						PORT2 <= (others => 'H');
					end if;
					wait until falling_edge(XTAL);
					ALE <= '0';
					wait until rising_edge(XTAL);
					PORT0 <= (others => 'Z');
					wait until falling_edge(XTAL);
					wait until rising_edge(XTAL);
					RD <= '0';
					wait until falling_edge(XTAL);
					wait until rising_edge(XTAL);
					wait until falling_edge(XTAL);
					wait until rising_edge(XTAL);
					wait until falling_edge(XTAL);
					wait until rising_edge(XTAL);
					RD <= '1';
                    if (EqualSLV(TMP_DATA(7 downto 0), PORT0)) then
                        DONE := true;
    				end if;
			 		wait until falling_edge(XTAL);
				end loop;
        		write(OUTPUT_LINE, s4);
        		write(OUTPUT_LINE, now);
        		writeline(OUTPUT_FILE, OUTPUT_LINE);
            when 'I' =>
                wait until falling_edge(IRQ_N);
			when others =>
			end case;
        end loop;
		wait until rising_edge(XTAL);
		wait until falling_edge(XTAL);
		PORT0 <= (others => 'Z');
        wait;
    end process CPU;

end architecture MODEL;
