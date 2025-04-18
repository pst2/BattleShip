# BattleShip
1. Giới thiệu

Battleship là một trò chơi hải chiến cổ điển, nơi hai người chơi sắp xếp hạm đội của mình trên một lưới ô vuông và cố gắng đoán vị trí tàu của đối phương để bắn trúng và đánh chìm chúng. Trò chơi được lập trình bằng C++ do ngôn ngữ này cung cấp hiệu suất cao và khả năng xử lý logic game mạnh mẽ.

Trong phiên bản C++ của Battleship, chương trình có thể sử dụng mảng hai chiều hoặc vector để đại diện cho bảng chơi, với các chức năng xử lý đặt tàu, bắn súng, kiểm tra trạng thái tàu và xác định người chiến thắng. Trò chơi có thể được triển khai dưới dạng giao diện console hoặc kết hợp với thư viện đồ họa như SDL2 để hiển thị trực quan.

2. Cách chơi

🎯 Mục tiêu

Tìm và bắn trúng toàn bộ các thuyền địch ẩn trên một bảng 10x10 trong số lượng lượt giới hạn và thời gian quy định.

🕹️ Cách chơi

Sau khi khởi chạy màn hình chờ sẽ hiện lên.

![Screenshot 2025-04-18 140152](https://github.com/user-attachments/assets/b11fa6f5-aa91-4c85-bfc3-8efee1abbd40)

Việc bạn cần làm là nhập tên và nhấn nút "Play" để bắt đầu.

![Screenshot 2025-04-18 140246](https://github.com/user-attachments/assets/670f872a-4710-444d-8c72-67c6f6c390d3)

Mỗi lượt chơi, bạn click chuột trái vào một ô trên bảng.
Nếu ô đó có thuyền, nó sẽ chuyển sang màu đỏ (X) – bạn đã bắn trúng!

![Screenshot 2025-04-18 140352](https://github.com/user-attachments/assets/fbbe9498-3746-49d4-bec1-cca0c3c2269b)


Nếu ô đó trống, nó sẽ chuyển sang màu xanh (O) – bạn đã bắn trượt.

![Screenshot 2025-04-18 140332](https://github.com/user-attachments/assets/ae34fb46-2249-41b1-9abc-84aa72e7b839)

Sau khi chiến thắng, hết lượt chơi hoặc hết thời gian chơi - bảng vị trí thực của thuyền sẽ hiện lên.

![Screenshot 2025-04-18 140439](https://github.com/user-attachments/assets/1eb5a066-cde0-4ab4-aa3a-f41f83340891)

 Game kết thúc khi:

Bạn bắn trúng hết toàn bộ thuyền → Chiến thắng!

Hết số lượt chơi hoặc hết thời gian → Thất bại!

Cuối cùng sẽ hiển thị màn hình chờ để bạn có thể chọn chơi lại hoặc thoát trò chơi.

![Screenshot 2025-04-18 140456](https://github.com/user-attachments/assets/505c5983-d3ad-4b27-8494-da256dd55d74)

⏱️ Quy tắc

Bạn có tối đa 36 lượt chơi.

Tổng thời gian để hoàn thành game là 90 giây.

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

revealShips(): Vị trí đồ họa của tàu.

isValidPlacement(): Vị trí hợp lệ để có thể đặt thuyền ngang hoặc dọc mà không bị trùng lặp.

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

+ SDL_MOUSEBUTTONUP, SDL_MOUSEBUTTONDOWN: Nhận tín hiệu khi người chơi ấn nút Play, Play Again, Quit.

⏱️ Quản lý thời gian & lượt chơi
Sử dụng chrono để đo thời gian thực từ khi game bắt đầu.

Nếu vượt quá TIME_LIMIT (90s) hoặc MAX_ATTEMPTS (36 lượt), game kết thúc.

Game kết thúc khi người chơi trúng đủ số ô thuyền.

🖼️ Ảnh nền & kết quả
Tải các ảnh:

+ "board.png" làm nền bảng chơi

![board](https://github.com/user-attachments/assets/a48b35de-c6db-4906-a7c4-cd7143d65694)

+ "Congratulation! You win!.png" khi thắng

![Congratulation! You win!](https://github.com/user-attachments/assets/69062d68-23ca-44fc-97b0-4f288e81a27d)

+ "You lost.png" khi thua do hết lượt

![You lost](https://github.com/user-attachments/assets/53e3df56-62de-4eb4-b449-fe3fb972795e)

+ "Times out!.png" khi thua do hết thời gian

![Times out!](https://github.com/user-attachments/assets/c6215e2e-925d-44d2-a05d-d46ebb0c2a5d)

+ "Tàu 2" hiển thị khi người chơi đánh chìm tàu

![tàu 2](https://github.com/user-attachments/assets/98d9f501-ba9b-4b10-a242-5a471f84b9b8)

🧪 Tệp liên quan:

+ graphics/: chứa các ảnh PNG

+ src/: chứa file code C++

+ assets/: ảnh nền, hình kết thúc

✅ Điểm nổi bật:
- Logic đơn giản, dễ hiểu – phù hợp cho người học SDL2 hoặc game 2D cơ bản.

- Có hệ thống kiểm tra thắng/thua, thời gian, lượt bắn.

- Giao diện trực quan bằng SDL2 – đủ sinh động cho một mini game.

- Tương tác dễ dàng qua bàn phím và chuột.
