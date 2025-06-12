# 2025년도 운영체제 2nd 과제

- 학번 : 20210543
- 이름 : 이준혁
- 학과 : 컴퓨터공학과

|[이준혁](https://github.com/beejh02)|
|:---:|
|<img src="https://avatars.githubusercontent.com/u/91623756?v=4" width="200" />|

---
- [05-24] : 기본 파일 추가, 코드 이해
- [05-25] : nalloc 구현, enqueue 부분 구현(최초 삽입, reply신호[success] 수신, head-tail 노드 포인트 조정), 내림차순 삽입, 코드 이해
- [05-26] : enqueue 구현(적절한 위치에 삽입), **`qtype.h 버그 리포트`**
- [05-27] : dequeue 구현
- [05-28] : range 구현, nclone 구현, enqueue의 버그 수정, 기능 단위 테스트
- [05-29] : <thread>, <mutex>, <condition_variable> 라이브러리 추가
- [05-30] : enqueue, range, dequeue Mutex 상태 관리, 코드 스타일링 개선, 버그 수정, 기능 단위 테스트
- [05-31] : 불필요한 코드 제거, 불필요한 라이브러리(cv) 제거, 리팩토링
- [06-02] : 코드에 대한 생각(힙 자료구조), Interface와 같은 역할을 하는 header 파일을 모두 구현해야하는지.
- [06-02] : 중복 key 값에 대한 추가 공지, 질의에 대한 응답과 공지 반영 확
- [06-03] : 초기화 -> 자료구조 교체, enqueue, dequeue, range, init, nalloc, nfree 등 기능 구현
- [06-03] : priority lock free queue 자료구조 사용
- [06-04] : 초기화 -> 코드 자체의 논리 오류 및 스파게티 코드로 인한 전면 수정, skip-list로 priority-queue 구현 시작
- [06-04] : **`깊은 복사(value)의 논리 오류 리포트`** -> value의 자료형을 알지 못하면 메모리공간을 못 정해줌.
- [06-05] : 초기화 -> 깊은복사 공지로 인한 초기화, 버그와 수정점 메모
- [06-06] : 초기화 -> 코드충돌로 인한 초기화(양방향 커밋 푸시 진행)
- [06-07] : enqueue, dequeue 등과 같은 기본 기능 구현, 중복 키 값 처리, C++스타일 C99스타일로 수정,
- [06-07] : range의 start-end 교체에 대한 고려 추가, free 과정에서 2중 메모리 해제하는 버그를 수정, 단위 테스트
- [06-08] : cout 출력으로 출력 간섭 mutex 추가로 수정 테스트 진행
- [06-09] : enqueue 무결성 테스트, dequeue 무결성 테스트, 속도 ns 단위로 테스트
- [06-10] : queue.cpp의 dequeue로 항상 참인 분기점 제거(리팩토링 수행) -> 상수시간 최적화
