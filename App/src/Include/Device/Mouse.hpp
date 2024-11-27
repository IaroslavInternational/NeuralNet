#pragma once

#include <queue>

class Mouse
{
	friend class MainWindow;
public:
	class Event
	{
	public:
		enum Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Invalid
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		bool wheelUp;
		bool wheelDown;
		int x;
		int y;
	public:
		Event()
			:
			type(Invalid),
			leftIsPressed(false),
			rightIsPressed(false),
			wheelUp(false),
			wheelDown(false),
			x(0),
			y(0)
		{}
		Event(Type type, const Mouse& parent)
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			wheelUp(parent.wheelUp),
			wheelDown(parent.wheelDown),
			x(parent.x),
			y(parent.y)
		{}
		bool IsValid() const
		{
			return type != Invalid;
		}
		Type GetType() const
		{
			return type;
		}
		std::pair<int, int> GetPos() const;
		int GetPosX() const;
		int GetPosY() const;
		bool LeftIsPressed() const;
		bool RightIsPressed() const;
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool LeftIsPressed() const;
	bool LeftIsReleased() const;
	bool RightIsPressed() const;
	bool IsWheelUp() const;
	bool IsWheelDown() const;
	bool IsInWindow() const;
	Mouse::Event Read();
	bool IsEmpty() const
	{
		return buffer.empty();
	}
	void Flush();
private:
	void OnMouseMove(int x, int y);
	void OnMouseLeave();
	void OnMouseEnter();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void TrimBuffer();
private:
	static constexpr unsigned int bufferSize = 4u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool leftIsReleased = false;
	bool rightIsPressed = false;
	bool wheelUp = false;
	bool wheelDown = false;
	bool isInWindow = false;
	std::queue<Event> buffer;
};