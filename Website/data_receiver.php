<?php
// Listen for data over post
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // get the csv line, and append
    $csvLine = $_POST['csv_line'];
    $file = fopen('data.csv', 'a');
    fwrite($file, $csvLine);
    fclose($file);
    
    // send response to client
    echo "CSV line received: " . $csvLine;
} else {
    echo "No data received";
}
?>
