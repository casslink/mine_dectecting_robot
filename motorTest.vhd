----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:38:18 04/01/2019 
-- Design Name: 
-- Module Name:    motorTest - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity motorTest is
    Port ( motor_1A : out  STD_LOGIC;
           motor_1B : out  STD_LOGIC;
           motor_2A : out  STD_LOGIC;
           motor_2B : out  STD_LOGIC;
			  SH_Din : in  STD_LOGIC;
           SH_CLK : in  STD_LOGIC;
           reg_clk : in  STD_LOGIC; 
			  encoder_1A : in  STD_LOGIC;
			  encoder_1B: in  STD_LOGIC;
			  clk : in STD_LOGIC;
			  PWM : in STD_LOGIC;
			  an_out : out STD_LOGIC_VECTOR(4 downto 0);
			  seg_out : out STD_LOGIC_VECTOR(6 downto 0);
			  dp : out  STD_LOGIC;
			  motor_on : in STD_LOGIC;
			  ARDUINO_RESET : out STD_LOGIC);
--			  senor_LED : out  STD_LOGIC_VECTOR (2 downto 0);
--			  trig : out STD_LOGIC_VECTOR (2 downto 0);
--			  echo : in STD_LOGIC_VECTOR (2 downto 0));
			  
end motorTest;

architecture Behavioral of motorTest is
--signal distance_0 : unsigned(7 downto 0) := (others => '0'); --left sensor distance
--signal distance_1 : unsigned(7 downto 0) := (others => '0'); --middle sensor distance
--signal distance_2 : unsigned(7 downto 0) := (others => '0'); --right sensor distance
signal shift_reg_data : STD_LOGIC_VECTOR(3 downto 0);
signal hold_reg : STD_LOGIC_VECTOR(3 downto 0);
signal encoder : STD_LOGIC;
signal cycles_counter : unsigned(7 downto 0) := (others => '0');
signal inches_counter : unsigned(7 downto 0) := (others => '0');
signal feet_counter : unsigned(7 downto 0):= (others => '0');
signal encoder_d1 : STD_LOGIC;
signal encoder_rise : STD_LOGIC;

component HEXon7segDisp is
    Port ( hex_data_in0 : in  STD_LOGIC_VECTOR (3 downto 0);
           hex_data_in1 : in  STD_LOGIC_VECTOR (3 downto 0);
           hex_data_in2 : in  STD_LOGIC_VECTOR (3 downto 0);
           hex_data_in3 : in  STD_LOGIC_VECTOR (3 downto 0);
           dp_in : in  STD_LOGIC_VECTOR (2 downto 0);
           seg_out : out  STD_LOGIC_VECTOR (6 downto 0);
           an_out : out  STD_LOGIC_VECTOR (3 downto 0);
           dp_out : out  STD_LOGIC;
           clk : in  STD_LOGIC);
end component;

begin

process(clk, motor_on, hold_reg, PWM)
begin 
	if rising_edge(clk) then
		if (motor_on = '0') then
				motor_1A <= '1';
				motor_1B <= '1';
				motor_2A <= '1';
				motor_2B <= '1';
		elsif (hold_reg = "1111") then
				motor_1A <= hold_reg(3);
				motor_1B <= hold_reg(2);
				motor_2A <= hold_reg(1);
				motor_2B <= hold_reg(0);
		else
				motor_1A <= PWM AND hold_reg(3);
				motor_1B <= PWM AND hold_reg(2);
				motor_2A <= PWM AND hold_reg(1);
				motor_2B <= PWM AND hold_reg(0);
		end if;
	end if;
end process;
	
encoder <= encoder_1A XOR encoder_1B;

process(clk)
begin
	if rising_edge(clk) then
		encoder_d1 <= encoder;
	end if;
end process;

encoder_rise <= encoder and not encoder_d1;

process(clk, encoder_rise) 
begin
	if rising_edge(clk) then
		if encoder_rise = '1' then
			if (cycles_counter = to_unsigned(104, 8)) then
				cycles_counter <= (others => '0');
			else
				cycles_counter <= cycles_counter + 1;
			end if;
		end if;
	end if;
end process;

process(clk, cycles_counter)
begin
	if rising_edge(clk) then
		if encoder_rise = '1' then
			if (inches_counter = to_unsigned(12, 8)) then
				inches_counter <= (others => '0');
			elsif (cycles_counter = to_unsigned(104, 8)) then
				inches_counter <= inches_counter + 1;
			end if;
		end if;
	end if;
end process;

process(clk, inches_counter)
begin
	if rising_edge(clk) then
		if encoder_rise = '1' then
			if (inches_counter = to_unsigned(12, 8)) then
				feet_counter <= feet_counter + 1;
			end if;
		end if;
	end if;
end process;

HEXon7segDispA : HEXon7segDisp
	port map (  hex_data_in0 => std_logic_vector(feet_counter(7 downto 4)),
					hex_data_in1 => std_logic_vector(feet_counter(3 downto 0)),
					hex_data_in2 => std_logic_vector(inches_counter(7 downto 4)),
					hex_data_in3 => std_logic_vector(inches_counter(3 downto 0)),
					dp_in => "000",  -- no decimal point
					seg_out => seg_out,
					an_out => an_out(3 downto 0),
					dp_out => dp,
					clk => clk
					);
an_out(4) <= '1';  -- 7 Seg Display Anode 4 is not used
	
process(SH_CLK, SH_Din)
begin
	if rising_edge(SH_CLK) then
		shift_reg_data <= (SH_DIN & shift_reg_data(3 downto 1));
	end if;
end process;

--Shifts data from shift register into hold register when reg_clk pulses
process(reg_clk, shift_reg_data)
begin
	if rising_edge(reg_clk) then
		hold_reg <= shift_reg_data;
	end if;
end process;




ARDUINO_RESET <= '1';


end Behavioral;

