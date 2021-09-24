# esteemaudit-patch
This is a third party patch for EsteemAudit NSA Exploit released in 2017

EsteemAudit exploit sends a fake SmartCard information packet to the vulnerable system. The vulnerable module whcih parses this malformed data , forwards it to memcpy() function.

<img src="https://trapmine.com/wp-content/uploads/2019/03/mempcy.png"/>

Size parameter of memcpy() func comes from the smartcard pci packet sent by EsteemAudit exploit. 

![alt text](https://trapmine.com/wp-content/uploads/2019/03/gpkcspbuffer.png)

So the size and source parameters of the memcpy() is controllable by attacker.  The destination of memcpy is a chunk at the size of 0x80 bytes. Thus, it is a simple buffer overflow issue.

To patch this issue , when vulnerable module gpkcsp.dll loaded in memory, we will hook memcpy at this location and will fix its size to 0x80 or even more safer to set it to 0x79 on the fly. To achieve this, after finding the offset of memcpy() in the memory, we should change the opcodes as following;

![alt text](https://trapmine.com/wp-content/uploads/2019/03/movecx80.png)

So the patch itself will be a DLL to inject into winlogon.exe process. When our DLL injected into the process , it should find offset of vulnerably memcpy first. In our tests, we see that it's always located at gpkcsp+0xE280 offset in Windows2003(SP1/SP2 ENG/TUR editions. We can calculate base address of gpkcsp module and add 0xE280 to find the vulnerable offset;

![alt_text](https://trapmine.com/wp-content/uploads/2019/03/patchadres.png)

After finding the offset, it is pretty straightforward. We should change original "mov ecx, dword_8176DDC" code with "mov ecx, 0x80" to fix the vulnerability. To do this , we should first make the memory RWX and then write our bytes to there;

![alt_text](https://trapmine.com/wp-content/uploads/2019/03/patch-opcode.png)

The last step is defining DLLMain() and call CreateThread to run our code inside the injected process;
![alt_text](https://trapmine.com/wp-content/uploads/2019/03/DllMain.png)

The patch is that simple. The difficult part is analyzing the vulnerable binary , locating it correctly and deciding on how to patch it.
