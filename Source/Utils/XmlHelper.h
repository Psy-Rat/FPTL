#pragma once

#include <string>
#include <fstream>

namespace FPTL
{
	namespace Utils
	{
		class XMLDomElement
		{
		public:
			XMLDomElement(const std::string& name)
			{
				this->tag = name;
				this->result = "<" + this->tag;
				this->f_attr_addition_enabled = true;
			}

			~XMLDomElement() {};

			// ��������� �������� � ������
			void addAttribute(const std::string& name, const std::string& value)
			{
				if (this->f_attr_addition_enabled)
					this->result = this->result + " " + name + "='" + value + "'";
				else
					throw "Attribute addition disabled";
			}

			// ��������� ��� � ����������� � ���������� ��� � ����� ��� ���������� �������
			void closeAttributes(std::ostream& output)
			{
				this->f_attr_addition_enabled = false;
				output << this->result + ">";
				this->result = "";
			}
			const std::string closeAttributes()
			{
				this->f_attr_addition_enabled = false;
				std::string returnable = this->result + ">";
				this->result = "";
				return returnable;
			}

			//������� ������� (���������� � �����, ���� ����� ������, ����������� ��� � ������������� ����������, ���� ����.)
			void close(std::ostream& output, const std::string& value = "")
			{
				const std::string& closetag = this->f_attr_addition_enabled ? ">" : "";
				this->result = this->result + closetag + value + "</" + this->tag + ">";
				output << this->result;
			}

			std::string close(const std::string& value = "")
			{
				const std::string& closetag = this->f_attr_addition_enabled ? ">" : "";
				this->result = this->result + closetag + value + "</" + this->tag + ">";
				return this->result;
			}


		private:
			std::string result;
			std::string tag;
			bool f_attr_addition_enabled;
		};

		static std::string OpenTag(const std::string& name)
		{
			return "<" + name + ">";
		}

		static void OpenTag(std::ostream& output, const std::string& name)
		{
			output << "<" << name << ">";
		}

		static std::string CloseTag(const std::string& name)
		{
			return "</" + name + ">";
		}

		static void CloseTag(std::ostream& output, const std::string& name)
		{
			output << "</" << name << ">";
		}
	}
}