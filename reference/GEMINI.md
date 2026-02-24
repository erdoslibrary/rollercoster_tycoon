# RollerCoaster Tycoon Native Clone (RCT-Native)

이 프로젝트는 크리스 소이어의 롤러코스터 타이쿤(RCT)을 현대적인 데스크톱 환경에서 구현하며, 원작의 고성능 시뮬레이션과 저수준 시스템 제어 철학을 계승하는 'Raw C++' 프로젝트입니다.

## 1. 상세 기술 명세 (Technical Specifications)

### 1.1 Core Engine
- **Language:** C++11 (광범위한 호환성 및 안정적인 언어 표준 준수)
- **Low-level Library:** SDL2 (Windowing, Input, Palette-based Texture Rendering)
- **Math:** Fixed-point arithmetic (고정 소수점 연산)을 기본으로 사용하여 결정론적 시뮬레이션 보장.
- **Memory:** 커스텀 풀 할당자(Pool Allocator)를 사용하여 수만 개의 엔티티(손님, 파티클) 생성/삭제 시 오버헤드 최소화. `std::vector`와 `std::unique_ptr`를 활용한 명확한 소유권 관리.

### 1.2 Rendering System
- **Software Renderer:** GPU 셰이더 대신 CPU에서 직접 픽셀 버퍼를 조작하는 소프트웨어 렌더링 방식 지향.
- **8-bit Palette System:** 원작 특유의 색상 변경(놀이기구 색상 등)을 재현하기 위해 8비트 인덱스 컬러 및 팔레트 스왑 시스템 구현.
- **Depth Sorting:** 아이소메트릭 뷰의 올바른 출력을 위해 'Back-to-Front' 정렬 알고리즘 및 타일 기반 가시성 관리.
- **Dirty Rectangles:** 화면 전체가 아닌 변경된 부분만 갱신하여 렌더링 효율 극대화.

### 1.3 Simulation & AI
- **Tick-based Update:** 렌더링 프레임과 독립된 고정 주기의 시뮬레이션 틱(예: 30Hz~60Hz) 운영.
- **Grid-based Pathfinding:** A* 알고리즘의 최적화 버전 혹은 원작 스타일의 간단하고 빠른 타일 기반 경로 탐색.
- **Physics:** 롤러코스터의 속력, 마찰력, 중력가속도(G-Force) 계산 로직 구현.

## 2. 단계별 구현 로드맵 (Phased Roadmap)

### Phase 1: Foundation (기초 다지기)
- [ ] CMake 빌드 환경 구축 및 SDL2 연동 (C++11 표준 강제).
- [ ] 고정 소수점 수학 라이브러리 및 커스텀 데이터 구조 정의.
- [ ] 기본 창 생성 및 팔레트 기반 8비트 픽셀 버퍼 렌더링 파이프라인 구축.

### Phase 2: World & Camera (월드 엔진)
- [ ] 타일 기반 아이소메트릭 그리드 시스템 (높이 데이터 포함).
- [ ] 스프라이트 엔진: 정렬된 스프라이트 출력 및 깊이 판정.
- [ ] 카메라 조작: 4방향 회전, 줌인/아웃, 드래그 이동.

### Phase 3: Entity System (엔티티 및 AI)
- [ ] 손님(Guest) 엔티티 정의 및 기본적인 타일 이동 로직.
- [ ] 경로 탐색 시스템: 도보 및 대기줄 인식.
- [ ] 간단한 손님 상태 머신(FSM) 및 욕구(배고픔, 화장실, 재미) 시스템.

### Phase 4: Rides & Physics (놀이기구와 물리)
- [ ] 트랙 조립 시스템: 직선, 곡선, 경사로 데이터 구조 설계.
- [ ] 열차 물리 엔진: 에너지 보존 법칙 기반의 속도 계산.
- [ ] G-포스 계산 및 놀이기구 수치(흥분도, 강도, 멀미도) 도출 로직.

### Phase 5: UI & Economy (UI 및 경제)
- [ ] 커스텀 창(Window) 시스템: 게임 내 팝업창 및 드래그 가능한 UI.
- [ ] 공원 관리 UI: 경제 시스템(입장료, 수익, 지출) 시뮬레이션.
- [ ] 건설 도구 및 지형 편집 인터페이스.

### Phase 6: Content & Persistence (콘텐츠 및 저장)
- [ ] 세이브/로드 시스템 (바이너리 포맷).
- [ ] 사운드 엔진 연동 (SDL_mixer).
- [ ] 시나리오 로딩 및 목표 달성 로직.

## 3. 개발 원칙 (Engineering Mandates)
1. **Zero External Dependency:** SDL2 외의 외부 라이브러리 사용을 지양하고 필요한 자료구조는 가급적 직접 구현한다.
2. **Deterministic Behavior:** 모든 시뮬레이션 결과는 어떤 환경에서도 동일해야 한다.
3. **Optimized for Mass Entities:** 수만 명의 손님이 있어도 CPU 점유율을 효율적으로 관리한다.
4. **Original Aesthetic:** 원작의 픽셀 감성과 UI 레이아웃을 최대한 존중한다.
5. **C++11 Compliance:** 최신 표준 기능을 지양하고 C++11 범위 내에서 안전하고 효율적인 코드를 작성한다.
