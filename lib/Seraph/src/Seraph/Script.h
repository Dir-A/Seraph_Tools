#pragma once
#include <span>
#include <memory>
#include <ranges>

#include <Rut/RxStr.h>
#include <Rut/RxJson.h>


namespace Seraph::Script
{
	enum ScenarioInstruction
	{
		Scenario_Text_Push,
		Scenario_Text_Format,
		Scenario_Font_Set_Color_R,
		Scenario_Font_Set_Color_G,
		Scenario_Font_Set_Color_B,
		Scenario_Font_Set_Width,
		Scenario_Font_Set_Heigh,
		Scenario_Un7,
		Scenario_Time_Wait,
		Scenario_Text_Allow_Color_Green,
		Scenario_Text_Draw_Layer,
		Scenario_Text_Allow_Vertical,
		Scenario_Text_Indent_Add = 0xE,
		Scenario_Se_Play,
		Scenario_Cursor_Allow_Click,
		Scenario_Text_Indent_At,
		Scenario_Next_Line = 0x14,
		Scenario_Input_Wait,
		Scenario_Text_Indent_Flag,
		Scenario_Next_Page,
		Scenario_Voice_Play,
		Scenario_Screen_Shake,
		Scenario_End = 0xFF
	};

	enum ExpressionInstruction
	{
		Expression_Static_Val0_Get,
		Expression_Static_Val1_Check,
		Expression_Static_Val2_Get_Type0,
		Expression_Static_Val2_Get_Type1,
		Expression_Static_Val2_Get_Type2,
		Expression_Dword_Get,
		Expression_Res_Get,
		Expression_Random_Val_Get,
		Expression_Addition,
		Expression_Subtraction,
		Expression_Multiplication,
		Expression_Division,
		Expression_Modulus,
		Expression_AND,
		Expression_OR,
		Expression_XOR,
		Expression_IF,
		Expression_Equal,
		Expression_NotEqual,
		Expression_LessThanEqual,
		Expression_GreaterThanEqual,
		Expression_LessThan,
		Expression_GreaterThan,
		Expression_LogicalAND,
		Expression_LogicalOR,
		Expression_PC_Set,
		Expression_End = 0xFF
	};


	class Parser
	{
	private:
		uint8_t m_ucOP = 0;
		uint32_t m_nPC = 0;
		const std::span<uint8_t> m_spCode;

	public:
		Parser(std::span<uint8_t> spCode);

	private:
		template<class T> auto Read();

	private:
		void SetPC(uint32_t uiPC);
		uint32_t GetPC() const;
		void SkipPC(uint32_t uiPC);
		bool NextInstr();
		uint8_t GetOPCode() const;

	private:
		Rut::RxJson::JValue ParseScenario();
		Rut::RxJson::JValue ParseExpression();
		std::wstring ParseScenarioInstrName() const;
		std::wstring ParseExpressionInstrName() const;
		Rut::RxJson::JValue ParseScenarioInstrParam();
		Rut::RxJson::JValue ParseExpressionInstrParam();

	public:
		Rut::RxJson::JValue Parse();

	};

	Parser::Parser(std::span<uint8_t> spCode) : m_spCode(spCode)
	{

	}

	template<class T> auto Parser::Read()
	{
		if constexpr (std::is_same_v<T, std::wstring>)
		{
			char* str_ptr = (char*)(m_spCode.data() + m_nPC);
			size_t str_len = strlen(str_ptr);
			m_nPC += str_len + 1;
			return Rut::RxStr::ToWCS({ str_ptr ,str_len }, 932);
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			char* str_ptr = (char*)(m_spCode.data() + m_nPC);
			size_t str_len = strlen(str_ptr);
			m_nPC += str_len + 1;
			return std::string{ str_ptr ,str_len };
		}
		else
		{
			T val = *(T*)(m_spCode.data() + m_nPC);
			m_nPC += sizeof(T);
			return val;
		}
	}

	void Parser::SetPC(uint32_t uiPC)
	{
		m_nPC = uiPC;
	}

	uint32_t Parser::GetPC() const
	{
		return m_nPC;
	}

	void Parser::SkipPC(uint32_t uiPC)
	{
		m_nPC += uiPC;
	}

	bool Parser::NextInstr()
	{
		m_ucOP = this->Read<uint8_t>();
		return m_ucOP != 0xFF ? true : false;
	}

	uint8_t Parser::GetOPCode() const
	{
		return m_ucOP;
	}

	Rut::RxJson::JValue Parser::ParseScenario()
	{
		Rut::RxJson::JArray codes;

		while (this->NextInstr())
		{
			Rut::RxJson::JObject instr;
			instr[L"opcode"] = this->GetOPCode();
			instr[L"command"] = this->ParseScenarioInstrName();
			instr[L"parameter"] = this->ParseScenarioInstrParam();
			codes.emplace_back(std::move(instr));
		}

		return codes;
	}

	Rut::RxJson::JValue Parser::ParseExpression()
	{
		Rut::RxJson::JArray codes;

		while (this->NextInstr())
		{
			Rut::RxJson::JObject instr;
			instr[L"opcode"] = this->GetOPCode();
			instr[L"command"] = this->ParseExpressionInstrName();
			instr[L"parameter"] = this->ParseExpressionInstrParam();
			codes.emplace_back(std::move(instr));
		}

		return codes;
	}
	
	std::wstring Parser::ParseScenarioInstrName() const
	{
		switch (this->GetOPCode())
		{
		case Scenario_Text_Push: return L"Scenario_Text_Push";
		case Scenario_Text_Format: return L"Scenario_Text_Format";
		case Scenario_Font_Set_Color_R: return L"Scenario_Font_Set_Color_R";
		case Scenario_Font_Set_Color_G: return L"Scenario_Font_Set_Color_G";
		case Scenario_Font_Set_Color_B: return L"Scenario_Font_Set_Color_B";
		case Scenario_Font_Set_Width: return L"Scenario_Font_Set_Width";
		case Scenario_Font_Set_Heigh: return L"Scenario_Font_Set_Heigh";
		case Scenario_Un7: return L"Scenario_Un7";
		case Scenario_Time_Wait: return L"Scenario_Time_Wait";
		case Scenario_Text_Allow_Color_Green: return L"Scenario_Text_Allow_Color_Green";
		case Scenario_Text_Draw_Layer: return L"Scenario_Text_Draw_Layer";
		case Scenario_Text_Allow_Vertical: return L"Scenario_Text_Allow_Vertical";
		case Scenario_Text_Indent_Add: return L"Scenario_Text_Indent_Add";
		case Scenario_Se_Play: return L"Scenario_Se_Play";
		case Scenario_Cursor_Allow_Click: return L"Scenario_Cursor_Allow_Click";
		case Scenario_Text_Indent_At: return L"Scenario_Text_Indent_At";
		case Scenario_Next_Line: return L"Scenario_Next_Line";
		case Scenario_Input_Wait: return L"Scenario_Input_Wait";
		case Scenario_Text_Indent_Flag: return L"Scenario_Text_Indent_Flag";
		case Scenario_Next_Page: return L"Scenario_Next_Page";
		case Scenario_Voice_Play: return L"Scenario_Voice_Play";
		case Scenario_Screen_Shake: return L"Scenario_Screen_Shake";
		case Scenario_End: return L"Scenario_End";
		default: throw std::runtime_error("Parser::ParseScenarioInstrName: Error Command!");
		}
	}

	std::wstring Parser::ParseExpressionInstrName() const
	{
		switch (this->GetOPCode())
		{
		case Seraph::Script::Expression_Static_Val0_Get: return L"Expression_Static_Val0_Get";
		case Seraph::Script::Expression_Static_Val1_Check: return L"Expression_Static_Val1_Check";
		case Seraph::Script::Expression_Static_Val2_Get_Type0: return L"Expression_Static_Val2_Get_Type0";
		case Seraph::Script::Expression_Static_Val2_Get_Type1: return L"Expression_Static_Val2_Get_Type1";
		case Seraph::Script::Expression_Static_Val2_Get_Type2: return L"Expression_Static_Val2_Get_Type2";
		case Seraph::Script::Expression_Dword_Get: return L"Expression_Dword_Get";
		case Seraph::Script::Expression_Res_Get: return L"Expression_Res_Get";
		case Seraph::Script::Expression_Random_Val_Get: return L"Expression_Random_Val_Get";
		case Seraph::Script::Expression_Addition: return L"Expression_Addition";
		case Seraph::Script::Expression_Subtraction: return L"Expression_Subtraction";
		case Seraph::Script::Expression_Multiplication: return L"Expression_Multiplication";
		case Seraph::Script::Expression_Division: return L"Expression_Division";
		case Seraph::Script::Expression_Modulus: return L"Expression_Modulus";
		case Seraph::Script::Expression_AND: return L"Expression_AND";
		case Seraph::Script::Expression_OR: return L"Expression_OR";
		case Seraph::Script::Expression_XOR: return L"Expression_XOR";
		case Seraph::Script::Expression_IF: return L"Expression_IF";
		case Seraph::Script::Expression_Equal: return L"Expression_Equal";
		case Seraph::Script::Expression_NotEqual: return L"Expression_NotEqual";
		case Seraph::Script::Expression_LessThanEqual: return L"Expression_LessThanEqual";
		case Seraph::Script::Expression_GreaterThanEqual: return L"Expression_GreaterThanEqual";
		case Seraph::Script::Expression_LessThan: return L"Expression_LessThan";
		case Seraph::Script::Expression_GreaterThan: return L"Expression_GreaterThan";
		case Seraph::Script::Expression_LogicalAND: return L"Expression_LogicalAND";
		case Seraph::Script::Expression_LogicalOR: return L"Expression_LogicalOR";
		case Seraph::Script::Expression_PC_Set: return L"Expression_PC_Set";
		case Seraph::Script::Expression_End: return L"Expression_End";
		default: throw std::runtime_error("Parser::ParseExpressionInstrName: Error!");
		};
	}

	Rut::RxJson::JValue Parser::ParseScenarioInstrParam()
	{
		Rut::RxJson::JObject param;

		switch (this->GetOPCode())
		{
		case Scenario_Text_Push:
		{
			param[L"value"] = this->Read<std::wstring>();
		}
		break;

		case Scenario_Text_Format:
		{
			throw std::runtime_error("Parser::ParseScenarioInstrParam: Error!");
		}
		break;

		case Scenario_Font_Set_Color_R:
		case Scenario_Font_Set_Color_G:
		case Scenario_Font_Set_Color_B:
		case Scenario_Font_Set_Width:
		case Scenario_Font_Set_Heigh:
		case Scenario_Un7:
		case Scenario_Time_Wait:
		case Scenario_Text_Allow_Color_Green:
		case Scenario_Text_Draw_Layer:
		case Scenario_Text_Allow_Vertical:
		case Scenario_Text_Indent_Add:
		case Scenario_Se_Play:
		case Scenario_Cursor_Allow_Click:
		case Scenario_Text_Indent_At:
		case Scenario_Screen_Shake:
		{
			param[L"value"] = this->Read<uint8_t>();
		}
		break;

		case Scenario_Next_Line:
		case Scenario_Input_Wait:
		case Scenario_Text_Indent_Flag:
		case Scenario_Next_Page:
			break;

		case Scenario_Voice_Play:
		{
			param[L"value"] = this->Read<int>();
		}
		break;

		default: throw std::runtime_error("Parser::ParseScenarioInstrParam: Error!");
		}

		return param;
	}

	Rut::RxJson::JValue Parser::ParseExpressionInstrParam()
	{
		Rut::RxJson::JObject param;

		switch (this->GetOPCode())
		{
		case Seraph::Script::Expression_Static_Val0_Get:
		{
			param[L"seq"] = this->Read<uint8_t>();
		}
		break;

		case Seraph::Script::Expression_Static_Val1_Check:
		{
			uint8_t type = this->Read<uint8_t>();
			param[L"flag"] = type;

			if (type == 1)
			{
				param[L"exp"] = this->ParseExpression();
			}
			else
			{
				param[L"seq0"] = this->Read<uint16_t>();
				param[L"seq1"] = this->Read<uint8_t>();
			}
		}
		break;

		case Seraph::Script::Expression_Static_Val2_Get_Type0:
		{
			param[L"seq"] = this->Read<uint16_t>();
		}
		break;

		case Seraph::Script::Expression_Static_Val2_Get_Type1:
		{
			param[L"seq"] = this->Read<uint16_t>();
			param[L"exp"] = this->ParseExpression();
		}
		break;

		case Seraph::Script::Expression_Static_Val2_Get_Type2:
		{
			param[L"un0"] = this->Read<uint16_t>();
			param[L"un1"] = this->Read<uint16_t>();
			param[L"exp0"] = this->ParseExpression();
			param[L"un2"] = this->Read<uint16_t>();
			param[L"exp1"] = this->ParseExpression();
		}
		break;

		case Seraph::Script::Expression_Dword_Get:
		{
			param[L"val"] = this->Read<int>();
		}
		break;

		case Seraph::Script::Expression_Res_Get:
		{
			param[L"res_table_seq"] = this->Read<uint16_t>();
			param[L"size0"] = this->Read<uint16_t>();
			param[L"exp_size1"] = this->ParseExpression();
			param[L"read_type"] = this->Read<uint8_t>();
			param[L"res_base"] = this->Read<uint16_t>();
			param[L"type"] = this->Read<uint16_t>();
		}
		break;

		case Seraph::Script::Expression_Random_Val_Get:
		{
			param[L"random_val"] = this->Read<int>();
		}
		break;

		case Seraph::Script::Expression_Addition:
		case Seraph::Script::Expression_Subtraction:
		case Seraph::Script::Expression_Multiplication:
		case Seraph::Script::Expression_Division:
		case Seraph::Script::Expression_Modulus:
		case Seraph::Script::Expression_AND:
		case Seraph::Script::Expression_OR:
		case Seraph::Script::Expression_XOR:
			break;

		case Seraph::Script::Expression_IF:
		{
			// ??
			throw std::runtime_error("Parser::ParseExpressionInstrParam: Error!");
		}
		break;

		case Seraph::Script::Expression_Equal:
		case Seraph::Script::Expression_NotEqual:
		case Seraph::Script::Expression_LessThanEqual:
		case Seraph::Script::Expression_GreaterThanEqual:
		case Seraph::Script::Expression_LessThan:
		case Seraph::Script::Expression_GreaterThan:
		case Seraph::Script::Expression_LogicalAND:
		case Seraph::Script::Expression_LogicalOR:
			break;

		case Seraph::Script::Expression_PC_Set:
		{
			param[L"pc"] = this->Read<int>();
		}
		break;

		case Seraph::Script::Expression_End:
			break;

		default: throw std::runtime_error("Parser::ParseExpressionInstrParam: Error!");
		}

		return param;
	}

	Rut::RxJson::JValue Parser::Parse()
	{
		uint8_t search_code[] = { 0xFF, 0x0F, 0x09, 0x1F, 0x00, 0x00, 0x47 };

		Rut::RxJson::JArray codes;
		for (; ;)
		{
			if (this->GetPC() >= m_spCode.size())
			{
				break;
			}

			if (memcmp(m_spCode.data() + this->GetPC(), search_code, sizeof(search_code)))
			{
				this->SkipPC(1);
				continue;
			}
			else
			{
				this->SkipPC(sizeof(search_code));
				codes.emplace_back(this->ParseScenario());
			}
		}

		return codes;
	}

}
