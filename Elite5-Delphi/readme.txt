
������ʾʹ���ڴ���صķ�ʽ�� Delphi ��ʹ�� slm_runtime_easy �ӿڣ�ʵ�������ع��ܽӿڼ�Ӳ�������ƿ��Ƶȹ��ܡ�

�ļ�˵����
BTMemory_x86.pas BTMemory��֧��Delphi XE��֮ǰ�汾����Delphi7���ã���
BTMemory_x86_64.pas BTMemory��֧��Delphi XE2��֮��汾��
slm_runtime_easy.pas runtime_easy API�ӿڶ���ͷ�ļ����ļ������� slm_runtime_easy.inc �ļ���
slm_runtime_easy.inc ��¼ slm_runtime_easy ��̬���ļ�ת���ɶ����Ƶ��������ݣ����� slm_runtime_easy.pas ʱ�ر��������ļ���
slm_runtime_easy_dll2inc.exe slm_runtime_easy ��̬���ļ�ת���� inc �����ļ����ߣ�����ʹ�÷������������"DLLתINC����ʹ��˵��"��

ʹ��˵����
1.�½����̣����� slm_runtime_easy.pas BTMemory_x86.pas BTMemory_x86_64.pas ����ĿĿ¼������ slm_runtime_easy��
2.���ݹ�������ƽ̨��32-bit �� 64-bit���ֱ�� {SDK��װĿ¼}\SDK\C\dll\x86 (��x64) Ŀ¼���� slm_runtime_easy_dev.dll �� slm_runtime_easy.dll ����ǰ��ĿĿ¼��ʹ�� slm_runtime_easy_dll2inc.exe ���߽� DLL ת��Ϊ inc �ļ���

ע�����
1.�л�����ѡ��Debug��Release��32-bit��64-bitʱ����Ҫ�ֶ�ɾ����Ŀ����Ŀ¼�µ�*.dcu�ļ����������Ϊ����Ĭ�����������ɵ�dcu����ִ�н����������һ�µ��������δ�������Ų��������ػᵼ����ʽ���氲װ��Ϊ�ɵ��԰棬���ͳ���ȫ�ԡ�
2.�����ʽ����ʱ slm_runtime_easy.inc ������ʹ�� slm_runtime_easy.dll ת�����ɺ�������ļ�����inc�������� slm_runtime_easy_dev.dll �ļ����ɣ��򲻾߱������Թ��ܣ�ͬע������1��

�������⣺
1.�л���������ƽ̨��32-bit -> 64-bit��ʱ��ʾ���´���
[DCC Fatal Error] F2048 Bad unit format: 'slm_runtime_easy.dcu' - Expected version: 23.0, Windows Unicode(x64) Found version: 23.0, Windows Unicode(x86)
���������ɾ������Ŀ¼���Ѿ����ɵ�.dcu�ļ������±��뼴�ɡ�
2.���г���ʱ���������򵯴���ʾ�����쳣��ַ�������� $C0000005��������̬���뵱ǰ��������ƽ̨��һ�£�32-bit��64-bit��������"ʹ��˵��"-"����2"��"ע������"���в��������¹������򼴿ɡ�
3.��װSDK��ʹ�� slm_runtime_easy_dll2inc.exe ���� inc �ļ�û�з�Ӧ��û���ļ�������SDKĬ�ϰ�װ��C�̣�һ�������C�̱����ǹ���ԱȨ�޲����������У��ǹ���ԱȨ������ʱ����ΪȨ�޲�������ļ�д��ʧ�ܵ��������鴫��DLL�ļ�·���Ƿ���Ҫ����ԱȨ�ޣ�����Ҫ��Ҫ����ԱȨ�ޣ��뽫 dll �ļ�����������Ȳ���Ҫ����ԱȨ��λ�ý���ת���������ɺ�� inc �ļ�������ʾ������Ŀ¼����ʹ�ù���ԱȨ������������ʾ��������ִ��ת�����

DLLתINC����ʹ��˵����
slm_runtime_easy_dll2inc.exe ����˵����
  ����1���������Ҫת����DLL�ļ�·��
  ����2����ѡ����ָ������INC�ļ�·��
  ʾ��1��ʹ��Ĭ������ļ�·��(${filename}.inc) slm_runtime_easy_dll2inc.exe slm_runtime_easy_dev.dll������ ${filename} ��ʾ�����ļ�·����
  ʾ��2��ָ������ļ�·�� slm_runtime_easy_dll2inc.exe slm_runtime_easy_dev.dll slm_runtime_easy.inc
  ע�⣺���� slm_runtime_easy.pas ����Ҫ�� inc �ļ��������� slm_runtime_easy.inc����������ʧ��
ʹ�ù��ߵ����ַ�ʽ��
1.��ק�ļ��� slm_runtime_easy_dll2inc.exe ����ͼ����ִ�У���ʹ��Ĭ������ļ�·��ת�������ļ���
2.ʹ��������ʾ�����ߣ�ִ��CMD���