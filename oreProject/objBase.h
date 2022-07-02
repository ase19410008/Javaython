#pragma once

namespace ore {
	enum class ObjType {
		NullType,
		ExpType,
		StmType,
		StmListType,
		RootType,
		InterpreterType,
		objTypeCount,
	};

	class ObjBase {
	protected:
		ObjBase(ObjType type);
	public:
		virtual ~ObjBase();
		ObjType getObjType() const;
		int getLineNumber() const;
		void setRuntimeLineNumber() const;
	private:
		struct Impl;
		Impl* pImpl;
	};
}