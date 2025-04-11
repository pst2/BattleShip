# BattleShip
1. Giới thiệu

Battleship là một trò chơi hải chiến cổ điển, nơi hai người chơi sắp xếp hạm đội của mình trên một lưới ô vuông và cố gắng đoán vị trí tàu của đối phương để bắn trúng và đánh chìm chúng. Trò chơi được lập trình bằng C++ do ngôn ngữ này cung cấp hiệu suất cao và khả năng xử lý logic game mạnh mẽ.

Trong phiên bản C++ của Battleship, chương trình có thể sử dụng mảng hai chiều hoặc vector để đại diện cho bảng chơi, với các chức năng xử lý đặt tàu, bắn súng, kiểm tra trạng thái tàu và xác định người chiến thắng. Trò chơi có thể được triển khai dưới dạng giao diện console hoặc kết hợp với thư viện đồ họa như SDL2 để hiển thị trực quan.

2. Cách chơi

🎯 Mục tiêu

Tìm và bắn trúng toàn bộ các thuyền địch ẩn trên một bảng 10x10 trong số lượng lượt giới hạn và thời gian quy định.

🕹️ Cách chơi

Mỗi lượt chơi, bạn click chuột trái vào một ô trên bảng.

Nếu ô đó có thuyền, nó sẽ chuyển sang màu đỏ (X) – bạn đã bắn trúng!

Nếu ô đó trống, nó sẽ chuyển sang màu xanh (O) – bạn đã bắn trượt.

 Game kết thúc khi:

Bạn bắn trúng hết toàn bộ thuyền → Chiến thắng!

Hết số lượt chơi hoặc hết thời gian → Thất bại!

⏱️ Quy tắc

Bạn có tối đa 32 lượt chơi.

Tổng thời gian để hoàn thành game là 65 giây.

Mỗi thuyền có chiều dài ngẫu nhiên từ 2 đến 3 ô và được đặt ngẫu nhiên trên bảng.

✅ Mẹo

Quan sát kỹ các vùng nghi ngờ đã có thuyền.

Khi bắn trúng 1 ô, hãy thử bắn các ô xung quanh – có thể là thân thuyền kéo dài.

4. Về source code game:

📂 Cấu trúc source code:

🎮 Gameplay & Logic
BOARD_SIZE, NUM_SHIPS, MAX_ATTEMPTS, TIME_LIMIT: Khai báo các thông số cơ bản như kích thước bảng, số tàu, giới hạn lượt chơi và thời gian.

Struct Ship: Đại diện cho một con tàu với vị trí, độ dài, và hướng đặt (ngang/dọc).

vector<vector<char>> board: Bảng trạng thái lưu vị trí các ô (~, S, X, O).

placeShips() & placeShip(): Tạo vị trí tàu ngẫu nhiên và đặt vào bảng nếu không trùng lặp.

isHit(): Kiểm tra người chơi bắn có trúng tàu không.

🖼️ Đồ họa (SDL2 + SDL2_image)
loadTexture(): Hàm hỗ trợ tải ảnh từ file .png để hiển thị lên renderer.

renderBoard() và Result(): Hàm vẽ bảng chơi lên màn hình, với mã màu cho từng trạng thái:

+ Đỏ (X) = Trúng thuyền

+ Xanh biển (O) = Trượt

+ Xanh lá (S) = Vị trí thuyền

+ Xám = Ô trống

🕹️ Xử lý sự kiện (SDL_Event)

+ SDL_MOUSEBUTTONDOWN: Nhận vị trí người chơi click, chuyển đổi từ pixel sang tọa độ bảng và xử lý theo trạng thái ô.

+ SDL_QUIT: Đóng game khi người dùng tắt cửa sổ.

+ SDL_KEYDOWN: Nhận tương tác bàn phím, phản hồi khi người chơi nhấn phím

⏱️ Quản lý thời gian & lượt chơi
Sử dụng chrono để đo thời gian thực từ khi game bắt đầu.

Nếu vượt quá TIME_LIMIT (90s) hoặc MAX_ATTEMPTS (36 lượt), game kết thúc.

Game kết thúc khi người chơi trúng đủ số ô thuyền.

🖼️ Ảnh nền & kết quả
Tải các ảnh:

+ "board.png" làm nền bảng chơi
![board](https://github.com/user-attachments/assets/cb1ba0b9-1857-49e9-b0a0-9af20a66d27f)

+ "Congratulation! You win!.png" khi thắng
![Congratulation! You win!](https://github.com/user-attachments/assets/c7fe97af-7447-498a-928f-5ba331ef3562)

+ "You lost.png" khi thua do hết lượt
  ![You lost](https://github.com/user-attachments/assets/097076a0-c001-4ff2-9cef-92c2b3c8647b)

+ "Times out!.png" khi thua do hết thời gian
![Times out!](https://github.com/user-attachments/assets/496d9f91-d9ca-42a9-93b8-40df21444fdd)

+ "Tàu 2" hiển thị khi người chơi đánh chìm tàu
![tàu 2](https://github.com/user-attachments/assets/98d9f501-ba9b-4b10-a242-5a471f84b9b8)

🧪 Tệp liên quan:
Không chia folder nhưng có thể tách thành:

+ graphics/: chứa các ảnh PNG

+ src/: chứa file code C++

+ assets/: ảnh nền, hình kết thúc

✅ Điểm nổi bật:
- Logic đơn giản, dễ hiểu – phù hợp cho người học SDL2 hoặc game 2D cơ bản.

- Có hệ thống kiểm tra thắng/thua, thời gian, lượt bắn.

- Giao diện trực quan bằng SDL2 – đủ sinh động cho một mini game.
