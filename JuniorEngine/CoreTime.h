#pragma once

class Time
{
public:
	// Возвращает время в секундах с момента старта движка
	static float GetTime();

	// Возвращает время, затраченное на отрисовку последнего кадра
	static float GetDeltaTime() { return m_DeltaTime; }

	// Метод, который мы будем вызывать в самом начале каждого кадра для обновления счётчиков
	static void Update();

private:
	static float m_DeltaTime; // переменная для хранения дельты
	static float m_LastFrameTime; // Время предыдущего кадра

};