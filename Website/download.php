<?php
$file = 'data.csv';

if (file_exists($file)) {
    // Set headers to force download
    header('Content-Description: File Transfer');
    header('Content-Type: application/csv');
    header('Content-Disposition: attachment; filename="'.basename($file).'";');
    header('Expires: 0');
    header('Cache-Control: must-revalidate');
    header('Pragma: public');
    header('Content-Length: ' . filesize($file));

    // Clear output buffer
    ob_clean();
    flush();

    // Read the file
    readfile($file);
    exit;
} else {
    echo "Error: File not found.";
}
?>
