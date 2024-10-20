<?php
// Check if the data is sent via POST method
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Get the CSV data from the request
    $csvLine = $_POST['csv_line'];
    
    // Save the CSV line to a file or process it as needed
    $file = fopen('test_data/fake_data.csv', 'a');
    fwrite($file, $csvLine);
    fclose($file);
    
    // Respond back to the client (ESP32)
    echo "CSV line received: " . $csvLine;
} else {
    echo "No data received";
}
?>
