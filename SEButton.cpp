/*
	���������� ��� ��������� ������ �� ������� ������� ��� ����������� ������.
	���������� ���������� ����������� ������� �������� � ���������� �������
	���� ��������� ��������� ���������� 12.09.2022�.
*/
#include "SEButton.h"
SEButton::SEButton()
{
	pin=0; // Pin �����
	analog_pin=false; // �������� �� pin ����������
	noize_filter_time=10; // ������ ����� � ���
	long_push_time=1200; // ����� ����������� ��������� ����� � ���
	dblclick_pause_time=700; // ������������ ����� ����� ����� 2 ���������
	min_level=128; // ���� ������� ������ ���� �������
  max_level=255; // ���� ����� ������ ���� �������
  gist_value=0; // �������� ����������� ��� ������� �����

	pressed=false; // ��������� ����� ����� ������� ����� (true - ��������/ false - ����������)
	on_change=false; // true, ���� ��������� ����� ����������
	on_press=false; // true, ���� ������ ������ ��� ����������
	on_release=false; // true, ���� ������ ������ ��� ������������.		
	longpressed=true; // true, ���� �������� on_longpress � ������ ��� ������. false ����� ����� on_release
	on_longpress=false; // true, ���� ������ ���������� �� ���������� �����			
	on_longrelease=false; // true, ���� ������ ������������ �� ����� ����������� �������
	cancel_future_on_click_dbl_click_long_press=false; //true, ���� ���������� �������� ������� ������� OnClick ��� OnDblClick ��� OnLongPress
	// ���������� �� ������ loop, ������� ����� �� ���� ������������
	inputchange_time=0; // �����, ����� ���������� ��������� ������
	last_loop_state=false; //GetIO ��� ����� ��� ������� ���������� ������ �������
	on_press_was=false; // true, ���� ������� on_press ���������
	on_release_was=false; // true, ���� ������� on_release ���������
	on_longpress_was=false; // true, ���� ������� on_longpress ���������
	on_longrelease_was=false; // true, ���� ������� on_longrelease ���������
	on_dbl_click_was=false; // true, ���� ������� on_dbl_click ���������
	on_first_click_time=0; // �����, ���������� ������ ����� ������� �������
	// ���������� �� ������ GetIOgis, ������� ����� �� ���� ������������
	GetIOgis_last_value=0;
};

// ������� �������� ������ (true - ������, false - ������)
bool SEButton::Pressed()
{
	return pressed;
};
// ��� ���������� ������� ������ (true - ������, false - ������)
bool SEButton::LongPressed()
{
	return on_longpress_was;
};
// true, ���� ������ ������ ��� ����������
bool SEButton::OnChange()
{
	return on_change;
};
// true, ���� ������ ������ ��� ����������
bool SEButton::OnPress()
{
	return on_press;
};
// true, ���� ������ ������ ��� ������������
bool SEButton::OnRelease()
{
	return on_release;
};
// true, ���� ������ ������ ��� ��������� ����������
bool SEButton::OnLongPress() 
{
	return on_longpress;
}
// true, ���� ������ ������ ��� ��������� ������������
bool SEButton::OnLongRelease() 
{
	return on_longrelease;
}
// true, ���� ��� ������� ��������������� click
bool SEButton::OnClick() 
{
	return on_click;
}
// true, ���� ��� ��c����� DblClick
bool SEButton::OnDblClick() 
{
	return on_dbl_click;
}
// �������� ���� ������� ������� OnClick ��� OnDblClick ��� OnLongPress
void SEButton::CancelFutureOnClickDblClickLongPress() // �������� ���� ������� ������� OnClick
{
	cancel_future_on_click_dbl_click_long_press=true; //true, ���� ���������� �������� ������� ������� OnClick ��� OnDblClick ��� OnLongPress
}

// �������������� ���������� � ��������� ������ ������
void SEButton::init()
{
	unsigned short temp_noize_filter_time=noize_filter_time; // ������ ����� � ���
	unsigned long temp_long_push_time=long_push_time; // ����� ����������� ���������/���������� ����� � ���/100
	unsigned long temp_dblclick_pause_time=dblclick_pause_time; // ������������ ����� ����� ����� 2 ���������
	noize_filter_time=1;
	long_push_time=2;
	dblclick_pause_time=1;
	loop();
	delay(3);
	loop(); // �������������� ��������� OnChange, OnPress, OnLongPress, OnLongRelease
	delay(2);
	loop(); // �������������� ��������� OnClick
	noize_filter_time=temp_noize_filter_time;
	long_push_time=temp_long_push_time;
	dblclick_pause_time=temp_dblclick_pause_time;
};

// �������, ������� ������ ����������� 1 ��� � loop() �������� ���������, ��� ���������� ��� ������� � ��������
void SEButton::loop() //�������, ������� ������ ����������� � loop() �������� ���������
{
  bool curr_state=GetIOgis(); // �������� �������� ������� ������ � ������ ����������� ��� �������� �������
	on_change=false; // true, ���� ��������� ����� ����������
	on_press=false; // true, ���� ������ ������ ��� ����������
	on_release=false; // true, ���� ������ ������ ��� ������������.		
	on_longpress=false; // true, ���� ������ ���������� �� ���������� �����			
	on_longrelease=false; // true, ���� ������ ������������ �� ����� ����������� �������
	on_click=false; //true, ���� ��� �������� ����
	on_dbl_click=false; //true, ���� ��� ������� ����
	
  if (last_loop_state==curr_state) // ���� ��������� ����� �� ���������� � �������� ����
  {
    if (millis()-inputchange_time>long_push_time) // ���� ���� �������/��������� ����� ������� ����������� ��������
			if (curr_state==true) 
			{
				if (!on_longpress_was)
				{
					if (!cancel_future_on_click_dbl_click_long_press)
						on_longpress=true; // true, ���� ������ ���������� �� ���������� �����		
					else cancel_future_on_click_dbl_click_long_press=false;
					on_longpress_was=true; // ������� ��� ���������
					on_longrelease_was=false; // ���������� ���� ���������������� �������
				}
			}
			else
			{
				if (!on_longrelease_was)
				{
					on_longrelease=true; // true, ���� ������ ������������ �� ����� ����������� �������
					on_longrelease_was=true; // ������� ��� ���������
					on_longpress_was=false; // ���������� ���� ���������������� �������
				}
			}
    
    if (millis()-inputchange_time>noize_filter_time) // ���� ���� �������/��������� ����� ����������� �������
    {
			if (curr_state==true) 
			{
				if (!on_press_was)
				{
					on_change=true; // ������ ���� ����������� ����������� ��������
					on_press=true; // ��������� ���������� �������
					on_press_was= true; // ������� ��� ���������
					on_release_was=false; // ���������� ���� ���������������� �������
					if (on_first_click_time!=0) // ���� ������� Double Click
					{
						if (!cancel_future_on_click_dbl_click_long_press) //true, ���� ���������� �������� ������� ������� OnClick ��� OnDblClick ��� OnLongPress
							on_dbl_click=true; //true, ���� ��� ������� ����
						else cancel_future_on_click_dbl_click_long_press=false;
						on_dbl_click_was=true; // true, ���� ������� on_dbl_click ���������
						on_first_click_time=0; // ���������� ����� �������� ������� �����
					}
				}
			}
			else
			{
				if (!on_release_was)
				{
					on_change=true; // ������ ���� ����������� ����������� ��������
					on_release=true; // ��������� ���������� �����������
					on_release_was=true; // ������� ��� ���������
					on_press_was=false; // ���������� ���� ���������������� �������
					if (!on_longpress_was) // ���� �� ���� ����������� ���������, ������ ��� click
					{
						if (!on_dbl_click_was) // ���� ��� �� ���������� ������ ����� Double Click
							on_first_click_time=millis(); // �������� ����� ������� ������� ������
						else
							on_dbl_click_was=false; // true, ���� ������� on_dbl_click ���������
					}
					else
						on_longpress_was=false; // ���������� ���� ������� Longpress. ��� ����� �� ��������� ������ Longpress, ���� ����� ���� ���� �������� ����������.
				}
			}
			pressed=curr_state;	// ����������� ������� ��������
    }

		// ����������� ����� ����� ������� click. ����� ��� ����� Double Click
		if (on_first_click_time!=0)
			if (millis()-on_first_click_time > dblclick_pause_time) // ���� ���� �������/��������� ����� ����������� �������
			{
				if (!cancel_future_on_click_dbl_click_long_press) //true, ���� ���������� �������� ������� ������� OnClick ��� OnDblClick ��� OnLongPress
					on_click=true; //true, ���� ��� �������� ����
				else cancel_future_on_click_dbl_click_long_press=false;					
				on_first_click_time=0; // ���������� ������ �������� ������� �����
			}
  }
  else
  {
    last_loop_state=curr_state;
    inputchange_time=millis(); // �������� ����� ��������� �����
  };
}
//--------------------------------------------------------------------------------------
// ���������� �������� ����������� ������ � ������ �����������
bool SEButton::GetIOgis()
{

  short temp_sh_min,temp_sh_max;

	if (analog_pin) // ���� ���� ����������
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