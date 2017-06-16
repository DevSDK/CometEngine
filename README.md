# Comet Engine


 ## Introduction
 
 _( 영알못이라 밑에 영어로 대충 써놓은건 아무말 대잔치. 젠장. )_
 
 #### 현제 개발 초기 (기획)단계입니다.
 #### 아직은 보여드릴게 많지 않지만 꾸준히 방문해 주시면 감사하겠습니다. ( _ _ ) 
 
  __Comet Engine__ 은 윈도우 게임 개발자들을 위해 개발되고 있는 오픈소스 게임엔진입니다. 
  
  Win32와 UWP를 같은 코드로 동작 하게끔 개발 할 예정입니다. 
  
  CometEngine은 Client - Server간의 개발을 쉽게끔 할 수 있도록 도와줄 것 입니다. 
  
  
  ## Engine Architecture

UWP와 Win32를 동시에 지원.

CometEngine Builder를 개발해 UWP, Win32로 빌드 할 수 있게끔 개발할 예정 -> 연구가 필요.

CometEngine의 모듈들:

	CometEngineCommon
    	CometEngine.dll
      	CometEngineCollision.dll
      	CometEngineFiles.dll
      	CometEngineGeometry.dll
      	CometEngineNetwork.dll
      	CometEngineLibs.dll
   	CometEngineClient
    	CometEngineDXRenderer.dll
      	CometEngineWin32.dll
   	CometEngineServer
    	CometEngineDB.dll
   
※__CometEngine Editor__ 는 추후에 추가 될 예정입니다.

 __Common__ 은 클라이언트와 서버 사이에 공통된 라이브러리, 도구, 엔진등등을 포함하고 있는 모듈 집합입니다.

 __Client__ 는 Windows 클라이언트를 위한 도구들을 포함하고 있는 모듈입니다. [Win32 DeskTop]

 __Server__ Multi-Core Processing, GPGPU를 활용해 효율적으로 서버를 처리하고, 데이터베이스관련 도구를 제공
 
 
 
 ## Design of Librarys
 
 난수를 생성 라이브러리를 자체 제공한다.  ex) 메르센 트위스터

 파일을 zip형태로 압축하거나, 리소스를 직접 컴파일함[암호화 지원]
 
• 잘 알려진 파일 형식 ex) PNG, JPEG, BMP 등등은 에디터에 불러와 파싱

• 자체적인 에니메이션 메커니즘이 필요 (리깅, 스켈레톤 등)
	
    -만약 필요할경우, Blender 같은 Util에서 만드는 데이터를 파싱할 필요 있음.
	
• 네트워크의 경우 Serializer를 제공, Bit 단위 전송을 최대한 지원.

     -C# 자체 통신은 사용하지 않고 C++ WinSock을 사용할 것, 다만 Wrapping에 주의
     
• 게임게발자가 로우레벨 시스템을 고려하지 않도록 설계하면서도 언제든지 로우레벨에 접근이 가능하도록 설계

• In-Game Profiler을 개발해 CPU,GPU, Memeory, Allocation등등을 수집 및 출력

• 필요할 경우 구조를 State Machine + Pipe Line로 모듈을 구성 
        
        - ex) Animation Pipeline, Resource Processing PipeLine, etc..
		

 
## Optimazation: 
	
SSE 명령어 사용 ( 다중 명령어 병렬처리 기법 ) - 하드웨어 가속 가능

	• __m128
	• 전용 레지스터 ( 16바이트 범위 보장 )
	• __declspec(align(16)) type []
	• MSDN 찾아볼것

메모리 Allocator구현 및 제공

	• 게임엔진 내에서 사용할 자료구조에 적용하면 효율적일 것으로 예상됨. ex) 렌더링 커멘드
	
파티클 시스템을 연구할 필요가 있음. Particle의 경우 자체를 State Machine으로 만들어 GPU 에서 처리하는 쪽으로 구상중

가능하다면 Collision Detection을 GPGPU로 구성

OpenMP 적극 활용 ( Loop 루틴, 산술연산 등)

## License:

 BSD License.
 
 
 <br>
 <br>
__ __


 # Comet Engine

 


## Introduction


__CometEngine__ is Open Source Game Engine for Windows Game Developers
   
Support Build Win32 and UWP using same code.

This project is in the early stages of development.

So, I can't show Engine Document not yet

## Engine Architecture

Support Build Win32 and UWP at the same code:

   Probably Develop 'CometEngine Builder' that module can UWP Win32 Build support
   
The Comet Engine Modules:

    CometEngineCommon
      CometEngine.dll
      CometEngineCollision.dll
      CometEngineFiles.dll
      CometEngineGeometry.dll
      CometEngineNetwork.dll
      CometEngineLibs.dll
    CometEngineClient
      CometEngineDXRenderer.dll
      CometEngineWin32.dll
    CometEngineServer
      CometEngineDB.dll
   
※__CometEngine Editor__ will add later.

The __Common__ is Common Modules Server and Client side 

The __Client__ is Client Modules for Windows Client [ Win32 DeskTop, UWP]

The __Server__ is Server Modules for Server side. That can support GPGUP, DB, Multi-Core  


## Design Of Librarys
   
•Provide Random Number Library.  ex) Mersenne Twister, or external library

•Compress Resources like zip file or Compile Resource. [Support Encryption]
   
   • Load to Editor and parsing data Well-Known File Format ex) PNG, JPEG, BMP and ETC 
   
   • Probably Need Own Animation Method ( Rigging, Skeleton )
   
       - Probably Need Parsing Data from well-known utils ex) Blender.
   
   • Network Module support Serializer, and make sure send a bit data.
   
 		- Do Not Use ".Net"and "C# Socket." Using C++ WinSock. and make sure Wrapping.
   • Make sure the Game Developer focus on High-Level. But The CometEngine provide Low-Level System
   
   • Develop In-Game Profiler. For  CPU,GPU, Memeory and Allocation Information Collect and Visualize
   
   • If I Need, module strcuture change to State Machine + Pipe Line
        
       - ex) Animation Pipeline, Resource Processing PipeLine, etc..


## Optimazation

Using SSE Operation and Hard-ware Acceleration

	• __m128

	• Own Register

	• __declspec(align(16)) type []

	•  Read More MSDN 
   
Memory Allocator Implementation and provide 

	• Using Engine Core Data-Structure ex) Render Command

I Need More Research The Particle System. My Opinion is Particle make to State Machine and processing GPU Level.

If I can, GPGPU Collision Detection 

Using OpenMP



## License:

 BSD License.
 
 
 <br>
 <br>
