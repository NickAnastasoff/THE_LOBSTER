<?php
// Read the CSV file (adjust file path as needed)
$data = array_map('str_getcsv', file('test_data/fake_data.csv'));

// Extract headers
$headers = array_shift($data);

// Convert Flood Detection from Yes/No to 1/0
foreach ($data as &$row) {
    $row[5] = strtolower($row[5]) === 'yes' ? 1 : 0;
}

// Prepare data for the default chart (e.g., temperature)
$dataPoints = [];
foreach ($data as $row) {
    $dateTime = strtotime($row[0]) * 1000;
    $temperatureValue = (float)$row[1];  // Assuming column 1 is temperature
    $dataPoints[] = array("x" => $dateTime, "y" => $temperatureValue);
}
?>

<!DOCTYPE HTML>
<html>
<head>
<script>
var data = <?php echo json_encode($data); ?>;

window.onload = function () {
    var chart = new CanvasJS.Chart("chartContainer", {
    animationEnabled: true,
    backgroundColor: "#2c2c2c",
    title:{
        text: "Temperature (C) Over Time",
        fontColor: "#ffffff"
    },
    axisY: {
        title: "Value",
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
        const newDataPoints = getDataPoints(data, columnIndex);
        chart.options.data[0].dataPoints = newDataPoints;
        chart.options.title.text = this.options[this.selectedIndex].text + ' Over Time';
        chart.render();
    });
}

function getDataPoints(data, columnIndex) {
    return data.map(row => {
        const dateTime = new Date(row[0]).getTime();
        let value = parseFloat(row[columnIndex]);
        return { x: dateTime, y: value };
    });
}
</script>
</head>
<body style="background-color: #1e1e1e; color: #ffffff;">
    <h1 style="text-align: center;">Ocean Sensor Data Dashboard</h1>
    <div style="display: flex; justify-content: space-between; padding: 20px;">
        <div style="text-align: center; flex: 1; border: 1px solid #444; margin: 5px; padding: 10px;">
            <h2>Summary</h2>
            <p>Temperature Highest: 25.6 °C</p>
            <p>Salinity Avg: 34.2 ppt</p>
        </div>
        <div style="text-align: center; flex: 1; border: 1px solid #444; margin: 5px; padding: 10px;">
            <h2>Most Active Parameter</h2>
            <p>pH Level Fluctuations</p>
            <p>Max Change: 0.5</p>
        </div>
    </div>
    <h2 style="text-align: center;">Graph Data</h2>
    <label for="dataSelect" style="padding-left: 20px;">Choose a parameter to graph:</label>
    <select id="dataSelect" style="background-color: #333; color: #ffffff; border: 1px solid #555;">
        <option value="1">Temperature (C)</option>
        <option value="2">pH</option>
        <option value="3">Salinity (ppt)</option>
        <option value="4">Accelerometer (M)</option>
    </select>

    <div id="chartContainer" style="height: 400px; width: 90%; margin: 0 auto; padding: 20px;"></div>
    <script src="https://cdn.canvasjs.com/canvasjs.min.js"></script>

    <!-- Option to download CSV -->
    <div style="text-align: center; padding: 20px;">
        <a href="download.php" style="background-color: #333; color: #ffffff; padding: 10px 20px; text-decoration: none; border: 1px solid #555;">Download CSV Data</a>
    </div>
</body>
</html>
