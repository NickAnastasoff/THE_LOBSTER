<?php
// Read the CSV file (adjust file path as needed)
$data = array_map('str_getcsv', file('test_data/fake_data.csv'));

// Extract headers
$headers = array_shift($data);

// Convert Flood Detection from Yes/No to 1/0
foreach ($data as &$row) {
    $row[5] = strtolower($row[5]) === 'yes' ? 1 : 0;
}

// Prepare data for each chart
$chartsData = [];
foreach (range(1, 7) as $columnIndex) {
    $dataPoints = [];
    foreach ($data as $row) {
        $dateTime = strtotime($row[0]) * 1000;
        $value = (float)$row[$columnIndex];
        $dataPoints[] = array("x" => $dateTime, "y" => $value);
    }
    $chartsData[] = $dataPoints;
}
?>

<!DOCTYPE HTML>
<html>
<head>
<script>
var data = <?php echo json_encode($data); ?>;
var chartsData = <?php echo json_encode($chartsData, JSON_NUMERIC_CHECK); ?>;

window.onload = function () {
    const chartTitles = [
        "Temperature (C) Over Time",
        "pH Level Over Time",
        "EC Over Time",
        "Flood Detection Over Time",
        "Wattage Over Time",
        "Air Temperature Over Time",
        "Humidity Over Time"
    ];
    
    const containerIds = [
        "temperatureChart",
        "phChart",
        "ecChart",
        "floodChart",
        "wattageChart",
        "airTempChart",
        "humidityChart"
    ];

    // Generate chart for each parameter
    containerIds.forEach((containerId, index) => {
        const chart = new CanvasJS.Chart(containerId, {
            animationEnabled: true,
            backgroundColor: "#2c2c2c",
            title: {
                text: chartTitles[index],
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
                dataPoints: chartsData[index],
                lineColor: "#00c3ff",
                markerColor: "#00c3ff",
            }]
        });
        chart.render();
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

    <div id="temperatureChart" style="height: 400px; width: 90%; margin: 0 auto; padding: 20px;"></div>
    <div id="phChart" style="height: 400px; width: 90%; margin: 0 auto; padding: 20px;"></div>
    <div id="ecChart" style="height: 400px; width: 90%; margin: 0 auto; padding: 20px;"></div>
    <div id="floodChart" style="height: 400px; width: 90%; margin: 0 auto; padding: 20px;"></div>
    <div id="wattageChart" style="height: 400px; width: 90%; margin: 0 auto; padding: 20px;"></div>
    <div id="airTempChart" style="height: 400px; width: 90%; margin: 0 auto; padding: 20px;"></div>
    <div id="humidityChart" style="height: 400px; width: 90%; margin: 0 auto; padding: 20px;"></div>

    <script src="https://cdn.canvasjs.com/canvasjs.min.js"></script>

    <!-- Option to download CSV -->
    <div style="text-align: center; padding: 20px;">
        <a href="download.php" style="background-color: #333; color: #ffffff; padding: 10px 20px; text-decoration: none; border: 1px solid #555;">Download CSV Data</a>
    </div>
</body>
</html>
