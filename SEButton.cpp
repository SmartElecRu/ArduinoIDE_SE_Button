/*
	Библиотека для упрощения работы со входами модулей при подключении кнопок.
	Библиотека позволяетс отслеживать события коротких и длительных нажатий
	Дата последних изменений библиотеки 12.09.2022г.
*/
#include "SEButton.h"
SEButton::SEButton()
{
	pin=0; // Pin входа
	analog_pin=false; // Является ли pin аналоговым
	noize_filter_time=10; // Фильтр шумов в млс
	long_push_time=1200; // Время длительного замыкания входа в млс
	dblclick_pause_time=700; // Максимальное время паузы между 2 нажатиями
	min_level=128; // Ниже данного порога вход замкнут
  max_level=255; // Выше этого порога вход замкнут
  gist_value=0; // Значение гистерезиса для каждого входа

	pressed=false; // Состояние входа после фильтра шумов (true - замкнуто/ false - разомкнуто)
	on_change=false; // true, если состояние входа изменилось
	on_press=false; // true, если кнопка только что замкнулась
	on_release=false; // true, если кнопка только что разомкнулась.		
	longpressed=true; // true, если сработал on_longpress и кнопка ещё нажата. false сразу после on_release
	on_longpress=false; // true, если кнопка замкнулась на длительное время			
	on_longrelease=false; // true, если кнопка разомкнулась на время длительного события
	cancel_future_on_click_dbl_click_long_press=false; //true, если необходимо отменить будущее событие OnClick или OnDblClick или OnLongPress
	// Переменные из фукции loop, которые могли бы быть статическими
	inputchange_time=0; // Время, когда изменилось состояние входоа
	last_loop_state=false; //GetIO для входа при прошлом выполнении данной функции
	on_press_was=false; // true, если событие on_press наступало
	on_release_was=false; // true, если событие on_release наступало
	on_longpress_was=false; // true, если событие on_longpress наступало
	on_longrelease_was=false; // true, если событие on_longrelease наступало
	on_dbl_click_was=false; // true, если событие on_dbl_click наступало
	on_first_click_time=0; // Время, размыкания кнопки после первого нажатия
	// Переменные из фукции GetIOgis, которые могли бы быть статическими
	GetIOgis_last_value=0;
};

// Текущее состоние кнопки (true - нажата, false - отжата)
bool SEButton::Pressed()
{
	return pressed;
};
// Идёт длительное нажатие кнопки (true - нажата, false - отжата)
bool SEButton::LongPressed()
{
	return on_longpress_was;
};
// true, если кнопка только что замкнулась
bool SEButton::OnChange()
{
	return on_change;
};
// true, если кнопка только что замкнулась
bool SEButton::OnPress()
{
	return on_press;
};
// true, если кнопка только что разомкнулась
bool SEButton::OnRelease()
{
	return on_release;
};
// true, если кнопка только что длительно замкнулась
bool SEButton::OnLongPress() 
{
	return on_longpress;
}
// true, если кнопка только что длительно разомкнулась
bool SEButton::OnLongRelease() 
{
	return on_longrelease;
}
// true, если сли натупил кратковременных click
bool SEButton::OnClick() 
{
	return on_click;
}
// true, если сли наcтупил DblClick
bool SEButton::OnDblClick() 
{
	return on_dbl_click;
}
// Сбросить одно будущее событие OnClick или OnDblClick или OnLongPress
void SEButton::CancelFutureOnClickDblClickLongPress() // Сбросить одно будущее событие OnClick
{
	cancel_future_on_click_dbl_click_long_press=true; //true, если необходимо отменить будущее событие OnClick или OnDblClick или OnLongPress
}

// Инициализаиует переменные и запускает начало обмена
void SEButton::init()
{
	unsigned short temp_noize_filter_time=noize_filter_time; // Фильтр шумов в млс
	unsigned long temp_long_push_time=long_push_time; // Время длительного замыкания/размыкания входа в млс/100
	unsigned long temp_dblclick_pause_time=dblclick_pause_time; // Максимальное время паузы между 2 нажатиями
	noize_filter_time=1;
	long_push_time=2;
	dblclick_pause_time=1;
	loop();
	delay(3);
	loop(); // Отрабатываются начальные OnChange, OnPress, OnLongPress, OnLongRelease
	delay(2);
	loop(); // Отрабатываются начальные OnClick
	noize_filter_time=temp_noize_filter_time;
	long_push_time=temp_long_push_time;
	dblclick_pause_time=temp_dblclick_pause_time;
};

// Функция, которая должна выполняться 1 раз в loop() основной программы, она генерируес все события с кнопками
void SEButton::loop() //Функция, которая должна выполняться в loop() основной программы
{
  bool curr_state=GetIOgis(); // Получаем значение нажатой кнопки с учётом гистерезиса без выдержки времени
	on_change=false; // true, если состояние входа изменилось
	on_press=false; // true, если кнопка только что замкнулась
	on_release=false; // true, если кнопка только что разомкнулась.		
	on_longpress=false; // true, если кнопка замкнулась на длительное время			
	on_longrelease=false; // true, если кнопка разомкнулась на время длительного события
	on_click=false; //true, если был короткий клик
	on_dbl_click=false; //true, если был двойной клик
	
  if (last_loop_state==curr_state) // Если состояние входа не изменилось с прошлого раза
  {
    if (millis()-inputchange_time>long_push_time) // Если вход замкнут/размокнут более времени длительного нажанитя
			if (curr_state==true) 
			{
				if (!on_longpress_was)
				{
					if (!cancel_future_on_click_dbl_click_long_press)
						on_longpress=true; // true, если кнопка замкнулась на длительное время		
					else cancel_future_on_click_dbl_click_long_press=false;
					on_longpress_was=true; // Событие уже наступало
					on_longrelease_was=false; // Сбрасываем флаг противоположного события
				}
			}
			else
			{
				if (!on_longrelease_was)
				{
					on_longrelease=true; // true, если кнопка разомкнулась на время длительного события
					on_longrelease_was=true; // Событие уже наступало
					on_longpress_was=false; // Сбрасываем флаг противоположного события
				}
			}
    
    if (millis()-inputchange_time>noize_filter_time) // Если вход замкнут/размокнут более предельного времени
    {
			if (curr_state==true) 
			{
				if (!on_press_was)
				{
					on_change=true; // ставим флаг наступления длительного действия
					on_press=true; // Произошло длительное нажатие
					on_press_was= true; // Событие уже наступало
					on_release_was=false; // Сбрасываем флаг противоположного события
					if (on_first_click_time!=0) // Если ожидаем Double Click
					{
						if (!cancel_future_on_click_dbl_click_long_press) //true, если необходимо отменить будущее событие OnClick или OnDblClick или OnLongPress
							on_dbl_click=true; //true, если был двойной клик
						else cancel_future_on_click_dbl_click_long_press=false;
						on_dbl_click_was=true; // true, если событие on_dbl_click наступало
						on_first_click_time=0; // Сбрасываем время ожидания второго клика
					}
				}
			}
			else
			{
				if (!on_release_was)
				{
					on_change=true; // ставим флаг наступления длительного действия
					on_release=true; // Произошло длительное отпускаение
					on_release_was=true; // Событие уже наступало
					on_press_was=false; // Сбрасываем флаг противоположного события
					if (!on_longpress_was) // Если не было длительного замыкания, значит это click
					{
						if (!on_dbl_click_was) // Если это не размыкание кнопки после Double Click
							on_first_click_time=millis(); // Засекаем время первого отжатия кнопки
						else
							on_dbl_click_was=false; // true, если событие on_dbl_click наступало
					}
					else
						on_longpress_was=false; // Сбрасываем флаг события Longpress. Без этого не сработает второй Longpress, если между ними было короткое размыкание.
				}
			}
			pressed=curr_state;	// Присваиваем текущее значение
    }

		// Отслеживаем время после первого click. Может это будет Double Click
		if (on_first_click_time!=0)
			if (millis()-on_first_click_time > dblclick_pause_time) // Если вход замкнут/размокнут более предельного времени
			{
				if (!cancel_future_on_click_dbl_click_long_press) //true, если необходимо отменить будущее событие OnClick или OnDblClick или OnLongPress
					on_click=true; //true, если был короткий клик
				else cancel_future_on_click_dbl_click_long_press=false;					
				on_first_click_time=0; // Сбрасываем таймер ожидания второго клика
			}
  }
  else
  {
    last_loop_state=curr_state;
    inputchange_time=millis(); // Засекаем время изменения входа
  };
}
//--------------------------------------------------------------------------------------
// Вычисление значения дискретного выхода с учетом гистерезиса
bool SEButton::GetIOgis()
{

  short temp_sh_min,temp_sh_max;

	if (analog_pin) // Если порт аналоговый
		analog_value=analogRead(pin)/4;
	else
		analog_value=(digitalRead(pin)==HIGH)?(max_level-gist_value):0;
	
  temp_sh_min=(short)min_level+(short)gist_value;
  temp_sh_max=(short)max_level-(short)gist_value;

  if (temp_sh_min<temp_sh_max)
    if ((analog_value>=temp_sh_min)&&(analog_value<=temp_sh_max))
      GetIOgis_last_value=false;
  if (analog_value<min_level) GetIOgis_last_value=true;
  if (analog_value>max_level) GetIOgis_last_value=true;
  return GetIOgis_last_value;
}