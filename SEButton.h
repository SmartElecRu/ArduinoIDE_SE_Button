/*
  SEButton.h - Library for buttons input.
  Created by Victor S. Bykov, 12.09.2022.
  Released into the public domain.
*/
#ifndef SEButton_h
#define SEButton_h
#include <Arduino.h>
class SEButton
{
  public:
		unsigned char pin; // Pin входа
		bool analog_pin; // Является ли pin аналоговым
		unsigned short noize_filter_time; // Фильтр шумов в млс
		unsigned long long_push_time; // Время длительного замыкания/размыкания входа в млс/100
		unsigned long dblclick_pause_time; // Максимальное время паузы между 2 нажатиями
		unsigned char min_level; // Ниже данного порога вход замкнут
    unsigned char max_level; // Выше этого порога вход замкнут
    unsigned char analog_value; // Считанное аналоговое значение
    unsigned char gist_value; // Значение гистерезиса для каждого входа
		SEButton(); // Начальное задение всех переменных
		bool Pressed(); // Текущее состоние кнопки (true - нажата, false - отжата)
		bool LongPressed(); // Идёт длительное нажатие кнопки (true - нажата, false - отжата)
		bool OnChange(); // true, если кнопка только что замкнулась
		bool OnPress(); // true, если кнопка только что замкнулась
		bool OnRelease(); // true, если кнопка только что разомкнулась
		bool OnLongPress(); // true, если кнопка только что длительно замкнулась
		bool OnLongRelease(); // true, если кнопка только что длительно разомкнулась
		bool OnClick(); // true, если сли наcтупил кратковременных click
		bool OnDblClick(); // true, если сли наcтупил DblClick
		void CancelFutureOnClickDblClickLongPress(); // Сбросить одно будущее событие OnClick или OnDblClick или OnLongPress
		void init(); // Инициализация пинов согласно начально заданным переменным
		void loop(); //Функция, которая должна выполняться в loop() основной программы
  private:
		bool GetIOgis(); // Выдает значение 1 - замкнута, 0 - разомкнута с учётом гистерезиса
		bool pressed; // Состояние входа после фильтра шумов (true - замкнуто/ false - разомкнуто)
		bool on_change; // true, если состояние входа изменилось
		bool on_press; // true, если кнопка только что замкнулась
		bool on_release; // true, если кнопка только что разомкнулась.		
		bool longpressed; // true, если сработал on_longpress и кнопка ещё нажата. false сразу после on_release	
		bool on_longpress; // true, если кнопка замкнулась на длительное время		
		bool on_longrelease; // true, если кнопка разомкнулась на время длительного события
		bool on_click; //true, если был короткий клик
		bool on_dbl_click; //true, если был двойной клик
		bool cancel_future_on_click_dbl_click_long_press; //true, если необходимо отменить будущее событие OnClick или OnDblClick или OnLongPress
		// Переменные из функции loop, которые могли бы быть статическими
		unsigned long inputchange_time; // Время, когда изменилось состояние входоа
		bool last_loop_state; //GetIO для входа при прошлом выполнении данной функции
		bool on_press_was; // true, если событие on_press наступало
		bool on_release_was; // true, если событие on_release наступало
		bool on_longpress_was; // true, если событие on_longpress наступало
		bool on_longrelease_was; // true, если событие on_longrelease наступало
		bool on_dbl_click_was; // true, если событие on_dbl_click наступало
		unsigned long on_first_click_time; // Время, размыкания кнопки после первого нажатия
		// Переменные из функции GetIOgis, которые могли бы быть статическими
		bool GetIOgis_last_value;
};
#endif