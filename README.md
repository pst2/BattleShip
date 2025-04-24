# 🚢 BattleShip - Trò chơi hải chiến cổ điển bằng C++

## 🎮 Giới thiệu

**BattleShip** là một trò chơi hải chiến cổ điển được lập trình bằng **C++** và sử dụng thư viện **SDL2** để hiển thị đồ họa, âm thanh và xử lý sự kiện. Trò chơi mang lại trải nghiệm chiến thuật đơn giản nhưng hấp dẫn – nơi người chơi cần tìm và đánh chìm các tàu địch được giấu ngẫu nhiên trên một bảng 10x10.

## 📥 Cài đặt & Tải game

1. Tải file **`BattleShipsGame.zip`** từ repo.
2. Giải nén và chạy file thực thi có tên `BattleShips` hoặc `start_game`.

### 🔧 Các điểm nổi bật từ code:

- **Đồ họa trực quan bằng SDL2**: sử dụng hình ảnh PNG cho nền và tàu, vẽ bảng bằng `SDL_RenderDrawRect` và `SDL_RenderFillRect`.
- **Âm thanh sống động** với SDL_mixer: bao gồm hiệu ứng trúng, trượt, thắng và thua, cùng nhạc nền lặp xuyên suốt game.
- **Giao diện người dùng**: người chơi nhập tên, bắt đầu game, nhận phản hồi từ lượt chơi và chọn chơi lại hoặc thoát.
- **Quản lý logic game**:
  - Bảng 10x10 được lưu bằng `vector<vector<char>>`.
  - Tàu có chiều dài 2 hoặc 3 ô, đặt ngẫu nhiên không chồng lặp.
  - Hệ thống tính số lượt bắn (`MAX_ATTEMPTS = 39`) và thời gian chơi (`TIME_LIMIT = 90 giây`).
  - Các trạng thái ô: `~` (trống), `S` (tàu), `X` (trúng), `O` (trượt).
- **Tính năng đồ họa**:
  - Hiển thị tên người chơi, thời gian còn lại, số lượt còn lại.
  - Hình ảnh kết thúc: Thắng (`Congratulation! You win!.png`), Thua (`You lost.png`, `Times out!.png`).
- **Tính năng tương tác**:
  - Nhập tên người chơi bằng bàn phím.
  - Giao diện nút "Play", "Play Again", "Quit" với hiệu ứng hover và click.

---

## 🕹️ Cách chơi

### 🎯 Mục tiêu
Bắn trúng và đánh chìm tất cả tàu địch trên bảng 10x10 trong **39 lượt chơi** hoặc **90 giây**.

### 📌 Hướng dẫn

- Khởi động game → Nhập tên → Nhấn nút **Play**.
- Click chuột trái vào ô trên bảng để bắn:
  - 🔴 **Màu đỏ (X)**: Trúng tàu
  - 🔵 **Màu xanh dương (O)**: Trượt

- Khi chiến thắng, hết lượt hoặc hết giờ, tất cả vị trí tàu sẽ được hiển thị.
- Sau đó, bạn có thể chọn **chơi lại** hoặc **thoát** game.

### 🔊 Âm thanh (SDL2_mixer)
Nhạc nền xuyên suốt trận đấu

Âm thanh cho từng sự kiện:

hit.wav: Bắn trúng

miss.wav: Bắn trượt

win.wav: Thắng

lose.wav, timeout.wav: Thua

### 🖼️ Đồ họa với SDL2
loadTexture(): Tải ảnh PNG làm nền và các phần tử.

renderBoard(): Hiển thị bảng với màu sắc trực quan.

renderText(): Hiển thị chữ với font Arial.

Màn hình chờ 

![Screenshot 2025-04-18 140152](https://github.com/user-attachments/assets/5a03fe9f-2c33-4f1a-b408-fd56eab76da4)

Khi bắn trượt 

![Screenshot 2025-04-18 140332](https://github.com/user-attachments/assets/bc71e61b-bcfc-4583-8d66-1535ffb9cb87)

Khi bắn trúng 

![Screenshot 2025-04-18 140352](https://github.com/user-attachments/assets/11767945-76f4-4610-bd41-ca17a8019b30)

Màn hình hiển thị vị trí của tàu

![Screenshot 2025-04-18 140439](https://github.com/user-attachments/assets/524f65b8-e587-4824-bcc7-bddccc383f27)

Màn hình chờ sau khi chơi xong một ván 

![Screenshot 2025-04-18 140456](https://github.com/user-attachments/assets/c942aadb-f17a-4cc7-8466-a5623c369f1c)

Hình ảnh tàu

![tàu 2](https://github.com/user-attachments/assets/dde317dc-2f0e-42f7-9dd0-52ee429b585b)


### ⏱️ Quy tắc

- Tối đa **39 lượt chơi**
- Tối đa **90 giây**
- Mỗi tàu có độ dài ngẫu nhiên từ **2 đến 3 ô**
- Tàu được đặt ngẫu nhiên, không đè lên nhau

### ✅ Mẹo chơi

- Hãy để ý các khu vực nghi ngờ.
- Khi trúng một ô, thử bắn các ô xung quanh – có thể là thân tàu!

