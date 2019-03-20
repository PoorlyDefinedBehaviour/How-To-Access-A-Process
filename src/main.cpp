#include <iostream>
#include <cstdarg>
#include <windows.h>

/**
 * print() só para não ter que ficar escrevendo cout e endl;
 * */
void print() {}
template <typename T, typename... Args>
void print(T head, Args... tail)
{
  std::cout << head << " ";
  print(tail...);
}

int main()
{
  /**
   * Encontrar a janela do programa;
   * O programa precisar estar aberto;
   * */
  HWND window = FindWindowA(NULL, "programabruxo");
  if (!window)
  {
    print("Couldn't find window");
    exit(EXIT_FAILURE);
  }

  /**
   * Pegar o id do programa;
   * */
  DWORD procID;
  GetWindowThreadProcessId(window, &procID);
  if (!procID)
  {
    print("Couldn't get process ID");
    exit(EXIT_FAILURE);
  }

  /**
   * Abrir uma handle para o programa, 
   * sem isso não da pra mexer na memoria dele
   * */
  HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
  if (!hProc)
  {
    print("Couldn't open a handle to the process");
    exit(EXIT_FAILURE);
  }

  /**
   * Escrever e ler a memória do processo
   * Se o endereço não for static, 
   * você vai precisar encontrar o endereço certo.
   * */
  const uintptr_t address = 0x404004;
  int value = 0;
  int buffer = 0;
  while (true)
  {
    ReadProcessMemory(hProc, (BYTE *)address, &buffer, sizeof(buffer), NULL);
    print("\nBuffer:", buffer);
    value += buffer;
    WriteProcessMemory(hProc, (BYTE *)address, &value, sizeof(value), NULL);
    Sleep(3000);
  }

  print(" Window:", window, "\n");
  print("Process ID:", procID, "\n");
  print("Process Handle:", hProc);
}