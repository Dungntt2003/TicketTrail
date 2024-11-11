CREATE DATABASE ticketrail;
USE ticketrail;

CREATE TABLE users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255),
    phone VARCHAR(255),
    email VARCHAR(255),
    password VARCHAR(255),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP
);

CREATE TABLE airlines (
    airline_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255),
    description VARCHAR(255),
    contact_email VARCHAR(255),
    contact_phone VARCHAR(255),
    logo_url VARCHAR(255)
);

CREATE TABLE airports (
    airport_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255),
    code VARCHAR(10),
    city VARCHAR(255),
    country VARCHAR(255)
);

CREATE TABLE airplanes (
    airplane_id INT PRIMARY KEY AUTO_INCREMENT,
    airline_id INT,
    capacity INT,
    FOREIGN KEY (airline_id) REFERENCES airlines(airline_id) ON DELETE CASCADE
);

CREATE TABLE seats (
    seat_id INT PRIMARY KEY AUTO_INCREMENT,
    seat_code VARCHAR(255),
    seat_class VARCHAR(255),
    price DECIMAL(10, 2),
    airplane_id INT,
    FOREIGN KEY (airplane_id) REFERENCES airplanes(airplane_id) ON DELETE CASCADE
);

CREATE TABLE flights (
    flight_id INT PRIMARY KEY AUTO_INCREMENT,
    flight_number VARCHAR(255),
    departure_airport_id INT,
    arrival_airport_id INT,
    departure_time DATETIME,
    arrival_time DATETIME,
    duration_minutes INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP,
    airplane_id INT,
    FOREIGN KEY (airplane_id) REFERENCES airplanes(airplane_id) ON DELETE CASCADE,
    FOREIGN KEY (departure_airport_id) REFERENCES airports(airport_id) ON DELETE CASCADE,
    FOREIGN KEY (arrival_airport_id) REFERENCES airports(airport_id) ON DELETE CASCADE
);

CREATE TABLE seats_flights (
    seat_flight_id INT PRIMARY KEY AUTO_INCREMENT,
    seat_id INT,
    flight_id INT,
    status INT,
    FOREIGN KEY (seat_id) REFERENCES seats(seat_id) ON DELETE CASCADE,
    FOREIGN KEY (flight_id) REFERENCES flights(flight_id) ON DELETE CASCADE
);

CREATE TABLE bookings (
    booking_id INT PRIMARY KEY AUTO_INCREMENT,
    status VARCHAR(255),
    booking_time TIMESTAMP,
    total_price DECIMAL(10, 2),
    updated_at TIMESTAMP,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    user_id INT,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
);

CREATE TABLE booking_details (
    booking_detail_id INT PRIMARY KEY AUTO_INCREMENT,
    booking_id INT,
    seat_id INT,
    FOREIGN KEY (seat_id) REFERENCES seats(seat_id) ON DELETE CASCADE,
    FOREIGN KEY (booking_id) REFERENCES bookings(booking_id) ON DELETE CASCADE
);

CREATE TABLE payments (
    payment_id INT PRIMARY KEY AUTO_INCREMENT,
    booking_id INT,
    amount DECIMAL(10, 2),
    payment_method VARCHAR(255),
    payment_status VARCHAR(255),
    payment_time TIMESTAMP,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    user_id INT,
    FOREIGN KEY (booking_id) REFERENCES bookings(booking_id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
);

CREATE TABLE feedbacks (
    feedback_id INT PRIMARY KEY AUTO_INCREMENT,
    flight_id INT,
    user_id INT,
    rating DECIMAL(2, 1),
    comment TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (flight_id) REFERENCES flights(flight_id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE
);


INSERT INTO airlines (name, description, contact_email, contact_phone, logo_url) VALUES
('Vietnam Airlines', 'Hãng hàng không quốc gia Việt Nam', 'info@vietnamairlines.com', '1900 1100', 'http://example.com/logo_vn.png'),
('Bamboo Airways', 'Hãng hàng không tư nhân đầu tiên tại Việt Nam', 'info@bambooairways.com', '1900 1800', 'http://example.com/logo_bamboo.png'),
('VietJet Air', 'Hãng hàng không giá rẻ tại Việt Nam', 'info@vietjetair.com', '1900 1886', 'http://example.com/logo_vietjet.png');

INSERT INTO airports (name, code, city, country) VALUES
('Nội Bài International Airport', 'HAN', 'Hà Nội', 'Việt Nam'),
('Tân Sơn Nhất International Airport', 'SGN', 'Thành phố Hồ Chí Minh', 'Việt Nam'),
('Đà Nẵng International Airport', 'DAD', 'Đà Nẵng', 'Việt Nam'),
('Phú Quốc International Airport', 'PQC', 'Phú Quốc', 'Việt Nam');

INSERT INTO airplanes (airline_id, capacity) VALUES
(1, 180),  -- Vietnam Airlines
(2, 190),  -- Bamboo Airways
(3, 200);  -- VietJet Air

INSERT INTO flights (flight_number, departure_airport_id, arrival_airport_id, departure_time, arrival_time, duration_minutes, airplane_id) VALUES
('VN123', 1, 2, '2024-12-01 10:00:00', '2024-12-01 12:00:00', 120, 1),  -- Vietnam Airlines
('BB456', 2, 3, '2024-12-01 15:00:00', '2024-12-01 17:00:00', 120, 2),  -- Bamboo Airways
('VJ789', 3, 4, '2024-12-01 09:00:00', '2024-12-01 10:30:00', 90, 3);   -- VietJet Air

INSERT INTO seats (seat_code, seat_class, price, airplane_id) VALUES
('1A', 'Business', 200.00, 1),
('1B', 'Business', 200.00, 1),
('2A', 'Economy', 100.00, 1),
('2B', 'Economy', 100.00, 1),
('3A', 'Economy', 100.00, 2),
('3B', 'Economy', 100.00, 2),
('4A', 'Economy', 50.00, 3),
('4B', 'Economy', 50.00, 3),
('5A', 'Economy', 50.00, 3);