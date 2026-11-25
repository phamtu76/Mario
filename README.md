# Mario 2D - C++ OOP + SFML

Starter project cho phần Thành viên 1 (Mario/Player) và Thành viên 3 (Map, Collision, Game, UI) theo bảng phân công nhóm.

## Công nghệ
- C++17
- SFML 2.5 hoặc 2.6
- CMake
- Visual Studio Code

> Code dùng API SFML 2.x (`sf::Event::KeyPressed`, `sf::Keyboard`). Không dùng SFML 3.x nếu chưa chuyển đổi API.

## Tính năng hiện có
- Mario di chuyển trái/phải, nhảy (nhả phím sớm thì nhảy thấp hơn), trọng lực.
- Coyote time và jump buffer giúp nhảy "dễ chịu" hơn.
- Va chạm với nền, bục, ống nước; không đi ra ngoài biên trái/phải.
- Bản đồ có 2 hố; rơi xuống hố sẽ mất một mạng.
- Camera cuộn theo nhân vật.
- Đồng xu (+10 điểm), cột cờ ở cuối màn để thắng.
- Menu, Pause, Game Over, Win; tự động Pause khi mất focus cửa sổ.
- Hệ thống 3 mạng, hồi sinh có thời gian bất tử.

Nhân vật, gạch, ống nước và cảnh nền được vẽ bằng hình SFML cơ bản làm placeholder.

## Cải tiến so với bản đầu
- `Config.h`: gom hằng số dùng chung (kích thước cửa sổ, ô, nhân vật, bước vật lý).
- Vật lý chạy theo bước cố định 1/120 giây, ổn định ở mọi FPS.
- `Collision`: đẩy ra theo trục xuyên nhỏ nhất, hết lỗi bị đẩy ngang khi rơi xuống mép bục.
- `Map`: nền và bục là các khối liền (không còn khe giữa ô), chỉ vẽ phần nằm trong màn hình.
- `Map::draw()` trước đây sửa thành viên trong hàm `const` (không biên dịch được), nay đã sửa.
- Không còn kẹt phím khi Pause hoặc mất focus.
- `UI`: căn giữa chữ, tự thử font hệ thống nếu chưa có `assets/font.ttf`.
- `CMakeLists.txt`: mặc định build Release, tự sao chép `assets` cạnh file `.exe`.

## Cài đặt trên Windows
1. Cài VS Code.
2. Cài extension **C/C++** và **CMake Tools**.
3. Cài CMake.
4. Cài SFML 2.6 (bản tương thích với compiler bạn dùng).
5. Đảm bảo compiler và SFML cùng toolchain (ví dụ MinGW 64-bit với SFML MinGW 64-bit).
6. Nếu CMake không tự tìm thấy SFML, đặt `SFML_DIR` trỏ tới thư mục chứa `SFMLConfig.cmake`, hoặc cấu hình `CMAKE_PREFIX_PATH`.

### Build bằng terminal
Mở terminal tại thư mục có `CMakeLists.txt`:

```bash
cmake -S . -B build
cmake --build build --config Release
```

Chạy:
- MinGW/Ninja thường tạo `build/MarioGame.exe`
- Visual Studio generator thường tạo `build/Release/MarioGame.exe`

Nếu Windows báo thiếu DLL, thêm thư mục `bin` của SFML vào PATH hoặc sao chép các DLL SFML cần thiết cạnh file `.exe`.

## Điều khiển
- A / ←: đi trái
- D / →: đi phải
- Space / W / ↑: nhảy (giữ lâu nhảy cao hơn)
- P hoặc ESC: tạm dừng / tiếp tục
- Enter: bắt đầu từ menu
- R: chơi lại khi Win/Game Over
- Đóng cửa sổ: thoát

## Font cho UI
Đặt một font TrueType tên `font.ttf` vào `assets/font.ttf` (ví dụ DejaVu Sans, hỗ trợ tiếng Việt). Nếu chưa có, game thử font hệ thống (Arial trên Windows); nếu vẫn không có thì game chạy nhưng không hiện chữ.

## Phân chia module
- Thành viên 1: `Mario.h`, `Mario.cpp`
- Thành viên 3: `Config.h`, `Map.h/.cpp`, `Collision.h/.cpp`, `Game.h/.cpp`, `UI.h/.cpp`, `main.cpp`, `CMakeLists.txt`

## Lưu ý khi tích hợp Thành viên 2
Thống nhất interface giữa các thành viên trước khi ghép. Có thể bổ sung `Enemy` và `Item` vào `Game::update()` / `Game::render()`, và đưa va chạm Mario-enemy vào module `Collision`. Các hàm công khai hiện tại được giữ nguyên tên; `Game::update()` giờ được gọi với bước thời gian cố định.

## Ghi chú kỹ thuật
Đây là nền tảng chạy thử, chưa có sprite animation thật, âm thanh, nhiều màn chơi, enemy hoặc item. Nên kiểm thử lại sau khi tích hợp.
