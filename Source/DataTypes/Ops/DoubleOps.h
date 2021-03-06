#pragma once

#include <istream>
#include <cmath>

#include "Ops.h"

namespace FPTL
{
	namespace Runtime
	{
		class DoubleOps : public BaseOps
		{
			DoubleOps() = default;

		public:
			static DoubleOps * get()
			{
				static DoubleOps ops;
				return &ops;
			}

			const Ops * combine(const Ops * aOther) const override
			{
				return aOther->withOps(this);
			}

			const Ops * withOps(const IntegerOps * aOther) const override
			{
				return get();
			}

			const Ops * withOps(const BooleanOps * aOther) const override
			{
				throw invalidOperation("combine");
			}

			const Ops * withOps(const DoubleOps * aOther) const override
			{
				return get();
			}

			TypeInfo getType(const DataValue &aVal) const override
			{
				static TypeInfo info("double");
				return info;
			}

			// Функции преобразования.
			long long toInt(const DataValue & aVal) const override
			{
				return static_cast<long long>(aVal.mDoubleVal);
			}

			double toDouble(const DataValue & aVal) const override
			{
				return aVal.mDoubleVal;
			}

			// Базисные функции.
			DataValue add(const DataValue & aLhs, const DataValue & aRhs) const override
			{
				return DataBuilders::createDouble(aLhs.getOps()->toDouble(aLhs) + aRhs.getOps()->toDouble(aRhs));
			}

			DataValue sub(const DataValue & aLhs, const DataValue & aRhs) const override
			{
				return DataBuilders::createDouble(aLhs.getOps()->toDouble(aLhs) - aRhs.getOps()->toDouble(aRhs));
			}

			DataValue mul(const DataValue & aLhs, const DataValue & aRhs) const override
			{
				return DataBuilders::createDouble(aLhs.getOps()->toDouble(aLhs) * aRhs.getOps()->toDouble(aRhs));
			}

			DataValue div(const DataValue & aLhs, const DataValue & aRhs) const override
			{
				const auto Right = aRhs.getOps()->toDouble(aRhs);
				if (Right == 0) throw divideByZero();
				return DataBuilders::createDouble(aLhs.getOps()->toDouble(aLhs) / Right);
			}

			DataValue abs(const DataValue & aArg) const override
			{
				return DataBuilders::createDouble(std::abs(aArg.mDoubleVal));
			}

			// Функции сравнения.
			DataValue equal(const DataValue & aLhs, const DataValue & aRhs) const override
			{
				return DataBuilders::createBoolean(aLhs.getOps()->toDouble(aLhs) == aRhs.getOps()->toDouble(aRhs));
			}

			DataValue less(const DataValue & aLhs, const DataValue & aRhs) const override
			{
				return DataBuilders::createBoolean(aLhs.getOps()->toDouble(aLhs) < aRhs.getOps()->toDouble(aRhs));
			}

			DataValue greater(const DataValue & aLhs, const DataValue & aRhs) const override
			{
				return DataBuilders::createBoolean(aLhs.getOps()->toDouble(aLhs) > aRhs.getOps()->toDouble(aRhs));
			}

			// Вывод в поток.
			void print(const DataValue & aVal, std::ostream & aStream) const override
			{
				aStream << aVal.mDoubleVal;
			}

			void write(const DataValue & aVal, std::ostream & aStream) const override
			{
				aStream << aVal.mDoubleVal;
			}

			DataValue read(std::istream & aStream) const override
			{
				DataValue val(get());
				aStream >> val.mDoubleVal;
				return val;
			}
		};

		inline DataValue DataBuilders::createDouble(const double aVal)
		{
			DataValue val(DoubleOps::get());
			val.mDoubleVal = aVal;
			return val;
		}
	}
}
