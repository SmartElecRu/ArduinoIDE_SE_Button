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
		unsigned char pin; // Pin �����
		bool analog_pin; // �������� �� pin ����������
		unsigned short noize_filter_time; // ������ ����� � ���
		unsigned long long_push_time; // ����� ����������� ���������/���������� ����� � ���/100
		unsigned long dblclick_pause_time; // ������������ ����� ����� ����� 2 ���������
		unsigned char min_level; // ���� ������� ������ ���� �������
    unsigned char max_level; // ���� ����� ������ ���� �������
    unsigned char analog_value; // ��������� ���������� ��������
    unsigned char gist_value; // �������� ����������� ��� ������� �����
		SEButton(); // ��������� ������� ���� ����������
		bool Pressed(); // ������� �������� ������ (true - ������, false - ������)
		bool LongPressed(); // ��� ���������� ������� ������ (true - ������, false - ������)
		bool OnChange(); // true, ���� ������ ������ ��� ����������
		bool OnPress(); // true, ���� ������ ������ ��� ����������
		bool OnRelease(); // true, ���� ������ ������ ��� ������������
		bool OnLongPress(); // true, ���� ������ ������ ��� ��������� ����������
		bool OnLongRelease(); // true, ���� ������ ������ ��� ��������� ������������
		bool OnClick(); // true, ���� ��� ��c����� ��������������� click
		bool OnDblClick(); // true, ���� ��� ��c����� DblClick
		void CancelFutureOnClickDblClickLongPress(); // �������� ���� ������� ������� OnClick ��� OnDblClick ��� OnLongPress
		void init(); // ������������� ����� �������� �������� �������� ����������
		void loop(); //�������, ������� ������ ����������� � loop() �������� ���������
  private:
		bool GetIOgis(); // ������ �������� 1 - ��������, 0 - ���������� � ������ �����������
		bool pressed; // ��������� ����� ����� ������� ����� (true - ��������/ false - ����������)
		bool on_change; // true, ���� ��������� ����� ����������
		bool on_press; // true, ���� ������ ������ ��� ����������
		bool on_release; // true, ���� ������ ������ ��� ������������.		
		bool longpressed; // true, ���� �������� on_longpress � ������ ��� ������. false ����� ����� on_release	
		bool on_longpress; // true, ���� ������ ���������� �� ���������� �����		
		bool on_longrelease; // true, ���� ������ ������������ �� ����� ����������� �������
		bool on_click; //true, ���� ��� �������� ����
		bool on_dbl_click; //true, ���� ��� ������� ����
		bool cancel_future_on_click_dbl_click_long_press; //true, ���� ���������� �������� ������� ������� OnClick ��� OnDblClick ��� OnLongPress
		// ���������� �� ������� loop, ������� ����� �� ���� ������������
		unsigned long inputchange_time; // �����, ����� ���������� ��������� ������
		bool last_loop_state; //GetIO ��� ����� ��� ������� ���������� ������ �������
		bool on_press_was; // true, ���� ������� on_press ���������
		bool on_release_was; // true, ���� ������� on_release ���������
		bool on_longpress_was; // true, ���� ������� on_longpress ���������
		bool on_longrelease_was; // true, ���� ������� on_longrelease ���������
		bool on_dbl_click_was; // true, ���� ������� on_dbl_click ���������
		unsigned long on_first_click_time; // �����, ���������� ������ ����� ������� �������
		// ���������� �� ������� GetIOgis, ������� ����� �� ���� ������������
		bool GetIOgis_last_value;
};
#endif