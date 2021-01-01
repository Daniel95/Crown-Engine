#pragma once

namespace Crown
{
	class EntityTracker
	{
	public:
		static int Get() { return s_Index; }
		static int Increment();
		static void Reset() { s_Index = 0; }
		
	private:
		static int s_Index;
		
	};
}
