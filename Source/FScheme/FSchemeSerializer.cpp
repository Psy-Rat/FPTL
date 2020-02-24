#include "FSchemeSerializer.h"
#include "Utils/FileStreamHelper.h"
#include "Utils/XmlHelper.h"

namespace FPTL
{
	namespace Runtime
	{
		void FSchemeSerializer::serialize(const FSchemeNode* node)
		{
			Utils::setPermissions(serialization_path);

			mFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				mFile.open(serialization_path, std::ios::out);
				tryVisit(node);
				mFile.close();
			}
			catch (const std::ios_base::failure&)
			{
				throw std::runtime_error(Utils::getfStreamError(mFile));
			}
		}

		void FSchemeSerializer::tryVisit(const FSchemeNode* node)
		{
			const auto res = visited.insert(std::make_pair(node, id));
			if (res.second)
			{
				id++;
				node->accept(this);
			}
			else
			{
				Utils::XMLDomElement ref = Utils::XMLDomElement("Reference");
				ref.addAttribute("refID", std::to_string(1 + res.first->second));
				ref.close(mFile);
			}
		}

		void FSchemeSerializer::visit(const FFunctionNode* node)
		{
			Utils::XMLDomElement func = Utils::XMLDomElement("Function");
			{
				func.addAttribute("ID", std::to_string(id));
				func.addAttribute("Hard", std::to_string(node->isLong()));
				func.addAttribute("Name", node->name());
				func.addAttribute("Line", std::to_string(node->line()));
				func.addAttribute("Column", std::to_string(node->col()));
			}
			func.close(mFile);
		}

		void FSchemeSerializer::visit(const FParallelNode* node)
		{
			Utils::XMLDomElement par = Utils::XMLDomElement("Parallel");
			{
				par.addAttribute("ID", std::to_string(id));
				par.addAttribute("Hard", std::to_string(node->isLong()));
				par.closeAttributes(mFile);
				{
					Utils::XMLDomElement left = Utils::XMLDomElement("Left");
					left.closeAttributes(mFile);
					tryVisit(node->left());
					left.close(mFile);
				}
				{
					Utils::XMLDomElement right = Utils::XMLDomElement("Right");
					right.closeAttributes(mFile);
					tryVisit(node->right());
					right.close(mFile);
				}
			}
			par.close(mFile);
		}

		void FSchemeSerializer::visit(const FSequentialNode* node)
		{
			Utils::XMLDomElement seq = Utils::XMLDomElement("Sequential");
			{
				seq.addAttribute("ID", std::to_string(id));
				seq.addAttribute("Hard", std::to_string(node->isLong()));
				seq.closeAttributes(mFile);
				{
					Utils::XMLDomElement first = Utils::XMLDomElement("First");
					first.closeAttributes(mFile);
					tryVisit(node->first());
					first.close(mFile);
				}

				{
					Utils::XMLDomElement second = Utils::XMLDomElement("Second");
					second.closeAttributes(mFile);
					tryVisit(node->second());
					second.close(mFile);
				}
			}
			seq.close(mFile);
		}

		void FSchemeSerializer::visit(const FConditionNode* node)
		{
			Utils::XMLDomElement cond = Utils::XMLDomElement("Condition");
			{
				cond.addAttribute("ID", std::to_string(id));
				cond.addAttribute("Hard", std::to_string(node->isLong()));
				cond.closeAttributes(mFile);

				{
					Utils::XMLDomElement rule = Utils::XMLDomElement("Rule");
					rule.closeAttributes(mFile);
					tryVisit(node->condition());
					rule.close(mFile);
				}

				{
					Utils::XMLDomElement then = Utils::XMLDomElement("Then");
					then.closeAttributes(mFile);
					tryVisit(node->trueBranch());
					then.close(mFile);
				}

				{
					Utils::XMLDomElement otherwise = Utils::XMLDomElement("Else");
					otherwise.closeAttributes(mFile);
					tryVisit(node->falseBranch());
					otherwise.close(mFile);
				}
			}
			cond.close(mFile);
		}

		void FSchemeSerializer::visit(const FTakeNode* node)
		{
			Utils::XMLDomElement take = Utils::XMLDomElement("Take");
			{
				take.addAttribute("ID", std::to_string(id));
				take.addAttribute("Hard", std::to_string(node->isLong()));
				take.addAttribute("Index", std::to_string(node->index()));
				take.addAttribute("Line", std::to_string(node->line()));
				take.addAttribute("Column", std::to_string(node->col()));
			}
			take.close(mFile);
		}

		void FSchemeSerializer::visit(const FConstantNode* node)
		{

			Utils::XMLDomElement constant = Utils::XMLDomElement("Constant");
			{
				std::stringstream value;
				node->data().getOps()->print(node->data(), value);

				constant.addAttribute("ID", std::to_string(id));
				constant.addAttribute("Hard", std::to_string(node->isLong()));
				constant.addAttribute("Type", std::string(node->type()));
				constant.addAttribute("Line", std::to_string(node->line()));
				constant.addAttribute("Column", std::to_string(node->col()));
				constant.addAttribute("Value", value.str());

			}
			constant.close(mFile);
		}

		void FSchemeSerializer::visit(const FStringConstant* node)
		{
			Utils::XMLDomElement string = Utils::XMLDomElement("String");
			{

				string.addAttribute("ID", std::to_string(id));
				string.addAttribute("Hard", std::to_string(node->isLong()));
				string.addAttribute("Type", std::string(node->type()));
				string.addAttribute("Line", std::to_string(node->line()));
				string.addAttribute("Column", std::to_string(node->col()));
				string.addAttribute("Value", node->str());

			}
			string.close(mFile);
		}

		void FSchemeSerializer::visit(const FScheme* scheme)
		{
			Utils::XMLDomElement sch = Utils::XMLDomElement("Scheme");
			{
				sch.addAttribute("ID", std::to_string(id));
				sch.addAttribute("Hard", std::to_string(scheme->isLong()));
				sch.addAttribute("Name", scheme->name());
				sch.closeAttributes(mFile);
				{
					Utils::XMLDomElement fnode = Utils::XMLDomElement("FirstNode");
					fnode.closeAttributes(mFile);
					tryVisit(scheme->firstNode());
					fnode.close(mFile);
				}

				{
					Utils::XMLDomElement defs = Utils::XMLDomElement("Definitions");
					defs.closeAttributes(mFile);
					for (auto element : scheme->mDefinitions)
					{
						Utils::XMLDomElement def_elem = Utils::XMLDomElement(element.first);
						def_elem.closeAttributes(mFile);
						tryVisit(element.second);
						def_elem.close(mFile);
					}
					defs.close(mFile);
				}

			}
			sch.close(mFile);
		}
	}
}
