import unittest
import subprocess
import base64

class TestASCII85Coder(unittest.TestCase):

    def test_encode_decode(self):
        # Генерируем случайные данные
        import random
        import string
        data = ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(100))

        # Кодирование с помощью base64 для сравнения
        base64_encoded = base64.a85encode(data.encode('utf-8')).decode('utf-8')

        # Кодирование с помощью вашего кодера ASCII85
        with open('input.txt', 'w') as f:
            f.write(data)
        subprocess.run(['./ascii85_tool', 'encode', 'input.txt', 'output.txt'])
        with open('output.txt', 'r') as f:
            ascii85_encoded = f.read()

        # Удаляем завершающий маркер '~>' перед сравнением
        ascii85_encoded = ascii85_encoded.rstrip('~>')

        # Сравнение результатов кодирования
        self.assertEqual(base64_encoded, ascii85_encoded)

        # Декодирование с помощью base64 для сравнения
        base64_decoded = base64.a85decode(base64_encoded.encode('utf-8')).decode('utf-8')

        # Декодирование с помощью вашего декодера ASCII85
        subprocess.run(['./ascii85_tool', 'decode', 'output.txt', 'decoded.txt'])
        with open('decoded.txt', 'r') as f:
            ascii85_decoded = f.read()

        # Сравнение результатов декодирования
        self.assertEqual(data, ascii85_decoded)

    def test_invalid_decode(self):
        # Генерируем некорректные данные для декодирования
        invalid_data = "InvalidBase64Data"

        # Попытка декодирования с помощью base64 вызовет исключение ValueError
        try:
            base64.a85decode(invalid_data.encode('utf-8'))
            self.fail("Expected ValueError to be raised")
        except ValueError as e:
            self.assertIsInstance(e, ValueError)

        # Попытка декодирования с помощью вашего декодера ASCII85 должна завершиться с кодом != 0
        with open('input.txt', 'w') as f:
            f.write(invalid_data)
        try:
            subprocess.run(['./ascii85_tool', 'decode', 'input.txt', 'output.txt'], check=True)
            self.fail("Expected non-zero exit code")
        except subprocess.CalledProcessError:
            pass

if __name__ == '__main__':
    unittest.main()

