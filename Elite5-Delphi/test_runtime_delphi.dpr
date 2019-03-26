program test_runtime_delphi;

{$APPTYPE CONSOLE}

{$R *.res}


uses
  {$IF CompilerVersion >= 23}
    // XE2 �����ϰ汾����ͷ�ļ�
    Winapi.Windows, System.SysUtils,
  {$ELSE}
    // XE���°汾������Delphi7����ͷ�ļ������������ռ�
    windows, SysUtils,
  {$IFEND}
  slm_runtime_easy;

const TEST_ENCRYPT_DATA_SIZE = 32;
  const SS_OK = 0;

PROCEDURE HEXDUMP(buff_in:PBYTE;
              len:SS_UINT32);

var
  i,j,cur,linemax,nprinted           :SS_UINT32;
  flag                               :boolean;
  testStr                            :string;

  buff                               :ARRAY[1..4096] of BYTE;
begin
  i := 0;
  j := 0;
  cur := 0;
  linemax := 0;
  nprinted := 0;


  if 0 = len then
  begin
    exit;
  end;

  for i := 1 to len do
  begin
    buff[i] := buff_in^;
    Inc(buff_in,1);
  end;

  i := 0;
  testStr := Format('hex_view = %u bytes'+#13#10+'offset 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F'+#13#10,[len]);
  Write(testStr);
  flag := TRUE;
  Repeat
    testStr := Format('%04x | ',[(nprinted div 16)]);
    Write(testStr);
    if nprinted >= len then
    begin
      flag := FALSE;
      break;
    end;
    linemax := 16;
    for j := 0 to linemax do
    begin
      cur := i + j;
      if cur >= len then
      begin
        flag := FALSE;
        Write('   ');
      end
      else
      begin
        testStr := Format('%02x ',[buff[cur]]);
        Write(testStr);
        nprinted := nprinted + 1;
      end;
    end;

    Writeln('| ');
    for j := 0 to linemax do
    begin
      cur := i + j;
      if cur >= len then
      begin
        flag := FALSE;
        break;
      end;

      if (ord(buff[cur]) > 30) and (ord(buff[cur]) < 127) then
      begin
        testStr := chr(buff[cur]);//Format('%c',[PChar(buff[cur])]);
        Write(testStr);
      end
      else
      begin
        Write('.');
      end;
    end;

    i := i + 16;
    Write(#13#10);
  Until flag = FALSE;

  Write(#13#10);

end;
    //��ʼ��
    // slm_init_easy('DB3B838B2E4F08F5C9EFCD1A5DD16341');

function test0(test_license_id:SS_UINT32):SS_UINT32;
var
  sts               :SS_UINT32;
  costime           :DWORD;

  tmpLicenseParam   :string;
  printfString      :string;
  printfAnsiString  :AnsiString;
  loginParam        :PANSICHAR;
  apiVersion        :SS_UINT32;
  ssVersion         :SS_UINT32;
  pszLicense        :PANSICHAR;
  hslm              :SLM_HANDLE_INDEX;
  inputBuf          :PBYTE;
  outputBuf         :PBYTE;
  testStr           :string;
  tmpBufLen         :SS_UINT32;
  retSize           :SS_UINT32;
  readSize          :SS_UINT32;
  readBuffer        :PBYTE;
  pszLockInfo       :PANSICHAR;
  pszSessionInfo    :PANSICHAR;
  pszLicenseInfo    :PANSICHAR;
  originalData      :PBYTE;
  encrypteData      :PBYTE;
  decrypteData      :PBYTE;
  writeBuffer       :PBYTE;
  mem_id            :SS_UINT32;
  mm_buffer         :PBYTE;
  deviceCert        :PBYTE;

  d2cfile           :TextFile;
  temp              :String;
  d2cstr            :PANSICHAR;
  msgstr            :PANSICHAR;

  verify_data       :PBYTE;
  sign_data         :PBYTE;
  sign_size         :SS_UINT32;

  login_param_st    :ST_LOGIN_PARAM;
  developer_id      :AnsiString;
label CLEAR,END_LAB;
begin
  //license_param := '{\"license_id\":' + UIntToStr(test_license_id) + ',\"time_out\":864000}';
  sts := SS_OK;
  // ��ӭ
  Writeln('hello'+#13#10+' test runtime api...login param is ' + tmpLicenseParam);

  // ��ʼ��Login����
  // ��ʼ��JSON��ʽ��������
  //tmpLicenseParam := Format('{"license_id":%u,"time_out":864000}',[test_license_id]);
  //loginParam := PAnsiChar(AnsiString(tmpLicenseParam));

  // ��ʼ���ṹ�崫������
  ZeroMemory(@login_param_st, SizeOf(login_param_st));
  login_param_st.size := SizeOf(login_param_st);
  login_param_st.license_id := test_license_id;
  login_param_st.timeout := 864000;

  // ��ð汾
  sts := slm_get_version_easy(apiVersion, ssVersion);
  printfString := Format('runtime version is %u',[apiVersion]);
  Writeln(printfString);

  // ��ʼ���������� ��������SS
  sts := slm_init_easy('DB3B838B2E4F08F5C9EFCD1A5DD16341');
  printfString := Format('slm_init_easy error=0x%08x',[sts]);
  Writeln(printfString);

  // �鿴������ID���������ó��ȣ�
  SetLength(developer_id, 17);
  slm_get_developer_id_easy(developer_id);
  Writeln(developer_id);
  SetLength(developer_id, 0); // �ͷ��ڴ�

  // ���APP�Ƿ񱻵���
  sts := slm_is_debug_easy();
  if 0 = sts then
  begin
    Writeln('_slm_is_debug_4 ok');
  end
  else
  begin
    printfString := Format('slm_is_debug_easy failed,error=0x%08x',[sts]);
    Writeln(printfString);
  end;

  // �������API
  pszLicense := slm_find_license_easy(test_license_id,JSON);
  sts := slm_get_last_error();
  if (SS_OK = sts) and (pszLicense <> nil) then
  begin
    printfString := Format(#13#10+'slm_find_license_easy ok '+#13#10+
                  ',licnese %u found result is json %s',[test_license_id,pszLicense]);
    Writeln(printfString);
    if pszLicense <> nil then
    begin
      slm_free_easy(pszLicense);
      pszLicense := nil;
    end;
  end
  else
  begin
    printfString := Format('slm_find_license_easy faild id = %u error = 0x%08x',[test_license_id,sts]);
    Writeln(printfString);
  end;

  // ��½���,������ʹ��ʱ��
  hslm := 0;
  //hslm := slm_login_easy(loginParam,JSON);    // ʹ��JSON��ʽ��¼(������ʹ�ã����ڰ�ȫ�����⣩
  hslm := slm_login_easy(@login_param_st, STRUCT);  //ʹ�ýṹ���¼�����飩
  sts := slm_get_last_error();
  printfString := Format('hslm = 0x%08x',[hslm]);
  Writeln(printfString);
  if SS_OK = sts then
  begin
    Writeln('slm_login_easy ok');
  end
  else
  begin
    printfString := Format('slm_login_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;


  Writeln('[��ʼ����]');
          sts := slm_led_control_easy(hslm, 0, 2, 1000);
  if SS_OK = sts then
  begin
    Writeln('slm_led_control_easy ok');
  end
  else
  begin
    printfString := Format('slm_led_control_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;
          sts := slm_led_control_easy(hslm, 0, 2, 1000);
  if SS_OK = sts then
  begin
    Writeln('slm_led_control_easy ok');
  end
  else
  begin
    printfString := Format('slm_led_control_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;

  // ����ִ��
  retSize := 0;
  tmpBufLen := 2048;
  GetMem(inputBuf,tmpBufLen);
  GetMem(outputBuf,tmpBufLen);
  ZeroMemory(inputBuf,tmpBufLen);
  ZeroMemory(outputBuf,tmpBufLen);
  testStr := '12345';
  CopyMemory(inputBuf,pbyte(pchar(testStr)),5);
  costime := GetTickCount;
  sts := slm_execute_static_easy(hslm,'sstest.evx',inputBuf,5,outputBuf,tmpBufLen,retSize);
  costime := GetTickCount - costime;
  FreeMem(inputBuf);
  FreeMem(outputBuf);
  if SS_OK = sts then
  begin
    printfString := Format('slm_execute_static_easy ok'+#13#10+',costtime is %u,outputBuf retSize=%u',[costime,retSize]);
    Writeln(printfString);
  end
  else
  begin
    printfString := Format('slm_execute_static_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;

  // ��ȡֻ����������С
  GetMem(readBuffer,1024);
  readSize := slm_user_data_getsize_easy(hslm,ROM);
  sts := slm_get_last_error();
  if 0 <> readSize then
  begin
    sts := slm_user_data_read_easy(hslm,ROM,readBuffer,0,readSize);
    if SS_OK = sts then
    begin
      printfString := Format('slm_user_data_read_easy ok readSize=%u',[readSize]);
      Writeln(printfString);
    end
    else
    begin
      printfString := Format('slm_user_data_read_easy failed error = 0x%08x',[sts]);
      Writeln(printfString);
    end;
  end;
  FreeMem(readBuffer);

  // ��ȡ����������С
  GetMem(readBuffer,20480);
  readSize := slm_user_data_getsize_easy(hslm,PUB);
  sts := slm_get_last_error();
  if 0 <> readSize then
  begin
    sts := slm_user_data_read_easy(hslm,PUB,readBuffer,0,readSize);
    if SS_OK = sts then
    begin
      printfString := Format('slm_user_data_read_easy pub ok readSize=%u',[readSize]);
      Writeln(printfString);
    end
    else
    begin
      printfString := Format('slm_user_data_read_easy pub failed error = 0x%08x',[sts]);
      Writeln(printfString);
      goto CLEAR;
    end;
  end;
  FreeMem(readBuffer);

  // �����ɻỰ�Ϸ�
  sts := slm_keep_alive_easy(hslm);
  if SS_OK = sts then
  begin
    Writeln('slm_keep_alive_easy ok ');
  end
  else
  begin
    printfString := Format('slm_keep_alive_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;

  // ��ȡ����Ϣ
  pszLockInfo := slm_get_info_easy(hslm,LOCK_INFO,JSON);
  sts := slm_get_last_error();
  if SS_OK = sts then
  begin
    Writeln('slm_get_info_easy ok ');
    printfString := Format('LOCK_INFO is %s',[pszLockInfo]);
    Writeln(printfString);
    slm_free_easy(pszLockInfo);
    pszLockInfo := nil;
  end
  else
  begin
    printfString := Format('slm_get_info_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;

  // ��ȡ�Ự��Ϣ
  pszSessionInfo := slm_get_info_easy(hslm,SESSION_INFO,JSON);
  sts := slm_get_last_error();
  if SS_OK = sts then
  begin
    Writeln('slm_get_info_easy ok ');
    printfString := Format('SESSION_INFO is %s',[pszSessionInfo]);
    Writeln(printfString);
    slm_free_easy(pszSessionInfo);
    pszSessionInfo := nil;
  end
  else
  begin
    printfString := Format('slm_get_info_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;

  // ��ȡ�����Ϣ
  pszLicenseInfo := slm_get_info_easy(hslm,LICENSE_INFO,JSON);
  sts := slm_get_last_error();
  if SS_OK = sts then
  begin
    Writeln('slm_get_info_easy ok ');
    printfString := Format('LICENSE_INFO is %s',[pszLicenseInfo]);
    Writeln(printfString);
    slm_free_easy(pszLicenseInfo);
    pszLicenseInfo := nil;
  end
  else
  begin
    printfString := Format('_slm_get_info_16 failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;

  // ���ܲ��Դ���
  GetMem(originalData,TEST_ENCRYPT_DATA_SIZE);
  GetMem(encrypteData,TEST_ENCRYPT_DATA_SIZE);
  GetMem(decrypteData,TEST_ENCRYPT_DATA_SIZE);
  ZeroMemory(originalData,TEST_ENCRYPT_DATA_SIZE);
  ZeroMemory(encrypteData,TEST_ENCRYPT_DATA_SIZE);
  ZeroMemory(decrypteData,TEST_ENCRYPT_DATA_SIZE);
  testStr := 'test_data1234567890';
  CopyMemory(originalData,pbyte(pchar(testStr)),Length(testStr));
  Writeln('before encryption: ' + testStr);
  sts := slm_encrypt_easy(hslm,originalData,encrypteData,TEST_ENCRYPT_DATA_SIZE);
  if SS_OK = sts then
  begin
    Writeln('_slm_encrypt_16 ok');
    //HEXDUMP(originalData,TEST_ENCRYPT_DATA_SIZE);
    //Writeln('');
    //HEXDUMP(encrypteData,TEST_ENCRYPT_DATA_SIZE);
  end
  else
  begin
    printfString := Format('slm_encrypt_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;


  // �����ɻỰ�Ϸ�
  sts := slm_keep_alive_easy(hslm);
  if SS_OK = sts then
  begin
    Writeln('slm_keep_alive_easy ok ');
  end
  else
  begin
    printfString := Format('slm_keep_alive_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;

  // ���ܲ��Դ���
  sts := slm_decrypt_easy(hslm,encrypteData,decrypteData,TEST_ENCRYPT_DATA_SIZE);
  if SS_OK = sts then
  begin
    Writeln('slm_decrypt_easy ok');
    //HEXDUMP(decrypteData,TEST_ENCRYPT_DATA_SIZE);
  end
  else
  begin
    printfString := Format('slm_decrypt_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;

  FreeMem(originalData);
  FreeMem(encrypteData);
  FreeMem(decrypteData);

  // �����ɻỰ�Ϸ�
  sts := slm_keep_alive_easy(hslm);
  if SS_OK = sts then
  begin
    Writeln('slm_keep_alive_easy ok ');
  end
  else
  begin
    printfString := Format('slm_keep_alive_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;

  // ��ȡ��д���Ĵ�С
  readSize := 0;
  readSize := slm_user_data_getsize_easy(hslm,RAW);
  sts := slm_get_last_error();
  if SS_OK = sts then
  begin
    printfString := Format('slm_user_data_getsize_easy ok readSize=%u',[readSize]);
    Writeln(printfString);
  end
  else
  begin
    printfString := Format('slm_user_data_getsize_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;

  if 0 <> readSize then
  begin
    testStr := 'ZYJTESTOK';
    sts := slm_user_data_write_easy(hslm,pbyte(pchar(testStr)),0,length(testStr));
    if SS_OK = sts then
    begin
      Writeln('slm_user_data_write_easy ok '+testStr);
    end
    else
    begin
      printfString := Format('slm_user_data_write_easy failed error = 0x%08x',[sts]);
      Writeln(printfString);
    end;

    GetMem(writeBuffer,1024);
    sts := slm_user_data_read_easy(hslm,RAW,writeBuffer,0, readSize);
    if SS_OK = sts then
    begin
      printfString := Format('slm_user_data_read_easy ok readSize=%u',[readSize]);
      Writeln(printfString);
    end
    else
    begin
      printfString := Format('slm_user_data_read_easy failed error = 0x%08x',[sts]);
      Writeln(printfString);
    end;
    FreeMem(writeBuffer);
  end;

  // �����ɻỰ�Ϸ�
  sts := slm_keep_alive_easy(hslm);
  if SS_OK = sts then
  begin
    Writeln('slm_keep_alive_easy ok ');
  end
  else
  begin
    printfString := Format('slm_keep_alive_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;

  // �ڴ��й�����,�ڴ�������ss-local�ڲ�
  mem_id := slm_mem_alloc_easy(hslm,2048);
  sts := slm_get_last_error();
  if SS_OK = sts then
  begin
    Writeln('slm_mem_alloc_easy ok');
  end
  else
  begin
    printfString := Format('slm_mem_alloc_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;

  // �ڴ�д
  testStr := 'testmm/���������й�....2048 BYTE.......';
  retSize := slm_mem_write_easy(hslm,mem_id,0,2048,pbyte(pchar(testStr)));
  sts := slm_get_last_error();
  if SS_OK = sts then
  begin
    Writeln('slm_mem_write_easy ok');
  end
  else
  begin
    printfString := Format('slm_mem_write_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;

  // �ڴ��
  GetMem(mm_buffer,2048+1);
  sts := slm_mem_read_easy(hslm,mem_id,0,2048,mm_buffer,retSize);
  if SS_OK = sts then
  begin
    Writeln('slm_mem_read_easy ok');
    //HEXDUMP(mm_buffer,retSize);
  end
  else
  begin
    printfString := Format('slm_mem_read_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;
  FreeMem(mm_buffer);

  // �ڴ��ͷ�
  sts := slm_mem_free_easy(hslm,mem_id);
  if SS_OK = sts then
  begin
    Writeln('slm_mem_free_easy ok');
  end
  else
  begin
    printfString := Format('slm_mem_free_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;

  // ��ȡ�豸֤��
  GetMem(deviceCert,2048);
  sts := slm_get_device_cert_easy(hslm,deviceCert,2048,retSize);
  if SS_OK = sts then
  begin
    Writeln('slm_get_device_cert_easy ok');
   // HEXDUMP(deviceCert,retSize);
  end
  else
  begin
    printfString := Format('slm_get_device_cert_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
    goto CLEAR;
  end;

  // update
  //assignfile(d2cfile, 'untitled.d2c');
  //reset(d2cfile);
  //readln(d2cfile, temp);
  //closefile(d2cfile);
  //
  //
  // update
  //assignfile(d2cfile, 'untitled.d2c');
  //reset(d2cfile);
  //readln(d2cfile, temp);
  //closefile(d2cfile);
  //
  //d2cstr := PAnsiChar(AnsiString(temp));
  //msgstr := slm_update_easy(d2cstr);
  //sts := slm_get_last_error();
  //if SS_OK = sts then
  //begin
  //  Writeln('_slm_update_8 ok');
  //end
  //else
  //begin
  //  printfString := Format('_slm_update_8 failed error = 0x%08x',[sts]);
  //  Writeln(printfString);
  //end;
  //if msgstr <> nil then
  //begin
  //  printfString := Format('error msg is %s',[msgstr]);
  //  Writeln(printfString);
  //  slm_free_easy(msgstr);
  //  msgstr := nil;
  //end ;

  // slm_error_format
    printfAnsiString := slm_error_format_easy(100, 2);
    Writeln(printfAnsiString);

// slm_verify_device
GetMem(verify_data, 256);
GetMem(sign_data, 512);
        strcopy(PANSICHAR(verify_data), PANSICHAR('SENSELOCK2222'));
        sign_size := 0;

        sts := slm_sign_by_device_easy(hslm, verify_data, 32+9, sign_data, 512, sign_size);
  if SS_OK = sts then
  begin
    Writeln('slm_sign_by_device_easy ok');
    //HEXDUMP(@sign_data[0],sign_size);
  end
  else
  begin
    printfString := Format('slm_sign_by_device_easy failed error = 0x%08x',[sts]);
    Writeln(printfString);
  end;

        Sleep(5000);
  Writeln('[��������]');

          sts := slm_led_control_easy(hslm, 1, 2, 1000);
  if SS_OK = sts then
  begin
    Writeln('slm_led_control_easy ok');
  end;


CLEAR:
  if 0 <> hslm then
  begin
    sts := slm_logout_easy(hslm);
    if SS_OK = sts then
    begin
      Writeln('_slm_logout_4 ok');

    end
    else
    begin
      printfString := Format('_slm_logout_4 failed error = 0x%08x',[sts]);
      Writeln(printfString);
      goto END_LAB;
    end;
  end;

END_LAB:
    slm_cleanup_easy();
    Writeln('test is over');


  result := sts;
end;

begin
  try
    { TODO -oUser -cConsole Main : Insert code here }
    Writeln(#13#10+'---------------------------------------------------------'+
                #13#10+'SS RUNTIME ������ʾȫ����'+#13#10+
            '---------------------------------------------------------'+#13#10);
    test0(0);
    Readln;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
