<?php
// Read the CSV file
$data = array_map('str_getcsv', file('test_data/fake_data.csv'));

// Extract headers
$headers = array_shift($data);

// Prepare data for the spline chart
$dataPoints = [];
foreach ($data as $row) {
    $dateTime = strtotime($row[0]) * 1000;
    $zValue = 0;
    if (preg_match('/\(([^,]+), ([^,]+), ([^)]+)\)/', $row[4], $matches)) {
        $zValue = (float)$matches[3];
    }
    $dataPoints[] = array("x" => $dateTime, "y" => $zValue);
}

?>

<!DOCTYPE HTML>
<html>
<head>
<script>
var data = <?php echo json_encode($data); ?>; // Add this line

window.onload = function () {
    var chart = new CanvasJS.Chart("chartContainer", {
    animationEnabled: true,
    backgroundColor: "#2c2c2c",
    title:{
        text: "Accelerometer Up/Down (Z Axis) Over Time",
        fontColor: "#ffffff"
    },
    axisY: {
        title: "Acceleration (m/s^2)",
        titleFontColor: "#ffffff",
        labelFontColor: "#ffffff",
        gridColor: "#555555",
        valueFormatString: "#0.##",
    },
    axisX: {
        title: "Date & Time",
        titleFontColor: "#ffffff",
        labelFontColor: "#ffffff",
        gridColor: "#555555",
        valueFormatString: "YYYY-MM-DD HH:mm",
    },
    data: [{
        type: "spline",
        markerSize: 5,
        xValueType: "dateTime",
        dataPoints: <?php echo json_encode($dataPoints, JSON_NUMERIC_CHECK); ?>,
        lineColor: "#00c3ff",
        markerColor: "#00c3ff",
    }]
});
    chart.render();

    // Event listener for changing chart data
    document.getElementById('dataSelect').addEventListener('change', function() {
        const columnIndex = parseInt(this.value);
        const newDataPoints = getDataPoints(data, columnIndex); // Now 'data' is defined
        chart.options.data[0].dataPoints = newDataPoints;
        chart.options.title.text = this.options[this.selectedIndex].text + ' Over Time';
        chart.render();
    });
}
function getDataPoints(data, columnIndex) {
    return data.map(row => {
        const dateTime = new Date(row[0]).getTime();
        let value;
        if (columnIndex === 4) {
            // Extract Z-axis value from accelerometer data
            const accelerometer = row[columnIndex];
            value = parseFloat(accelerometer.match(/\(([^,]+), ([^,]+), ([^)]+)\)/)[3]);
        } else {
            value = parseFloat(row[columnIndex]);
        }
        return { x: dateTime, y: value };
    });
}
</script>
</head>
<body style="background-color: #1e1e1e; color: #ffffff;">
    <h1>Environment Data</h1>
    <table style="width: 100%; border-collapse: collapse; color: #ffffff;">
        <thead>
            <tr>
                <?php foreach ($headers as $header): ?>
                    <th style="border: 1px solid #555; padding: 10px; text-align: center;"><?php echo htmlspecialchars($header); ?></th>
                <?php endforeach; ?>
            </tr>
        </thead>
        <tbody>
            <?php foreach ($data as $row): ?>
                <tr>
                    <?php foreach ($row as $cell): ?>
                        <td style="border: 1px solid #555; padding: 10px; text-align: center;"><?php echo htmlspecialchars($cell); ?></td>
                    <?php endforeach; ?>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>

    <h2>Graph Data</h2>
    <label for="dataSelect">Choose a parameter to graph:</label>
    <select id="dataSelect" style="background-color: #333; color: #ffffff; border: 1px solid #555;">
        <option value="4">Accelerometer (Up/Down - Z Axis)</option>
        <option value="1">Temperature (C)</option>
        <option value="2">pH</option>
        <option value="3">Salinity (ppt)</option>
        <option value="6">Wattage (W)</option>
    </select>

    <div id="chartContainer" style="height: 370px; width: 100%;"></div>
    <script src="https://cdn.canvasjs.com/canvasjs.min.js"></script>
    
</body>
</html>