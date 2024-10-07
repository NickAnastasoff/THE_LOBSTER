<?php
// Read the CSV file
$data = array_map('str_getcsv', file('test_data/fake_data.csv'));

// Extract headers
$headers = array_shift($data);

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Environment Data</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        table {
            width: 100%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid black;
        }
        th, td {
            padding: 10px;
            text-align: center;
        }
    </style>
</head>
<body>
    <h1>Environment Data</h1>
    <table>
        <thead>
            <tr>
                <?php foreach ($headers as $header): ?>
                    <th><?php echo htmlspecialchars($header); ?></th>
                <?php endforeach; ?>
            </tr>
        </thead>
        <tbody>
            <?php foreach ($data as $row): ?>
                <tr>
                    <?php foreach ($row as $cell): ?>
                        <td><?php echo htmlspecialchars($cell); ?></td>
                    <?php endforeach; ?>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>

    <h2>Graph Data</h2>
    <label for="dataSelect">Choose a parameter to graph:</label>
    <select id="dataSelect">
        <option value="4">Accelerometer (Up/Down - Z Axis)</option>
        <option value="1">Temperature (C)</option>
        <option value="2">pH</option>
        <option value="3">Salinity (ppt)</option>
        <option value="6">Wattage (W)</option>
    </select>

    <canvas id="dataChart" width="400" height="200"></canvas>
    <script>
        document.addEventListener("DOMContentLoaded", function() {
            const data = <?php echo json_encode($data); ?>;
            const labels = data.map(row => row[0]);

            function getData(columnIndex) {
                return data.map(row => {
                    if (columnIndex === 4) {
                        // For accelerometer, extract Z-axis value
                        const accelerometer = row[4];
                        return parseFloat(accelerometer.match(/\(([^,]+), ([^,]+), ([^)]+)\)/)[3]);
                    } else {
                        return parseFloat(row[columnIndex]);
                    }
                });
            }

            const ctx = document.getElementById('dataChart').getContext('2d');
            let chart = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: labels,
                    datasets: [{
                        label: 'Accelerometer (Up/Down - Z Axis)',
                        data: getData(4),
                        borderColor: 'rgba(75, 192, 192, 1)',
                        borderWidth: 2,
                        fill: false
                    }]
                },
                options: {
                    responsive: true,
                    scales: {
                        x: {
                            title: {
                                display: true,
                                text: 'Date & Time'
                            }
                        },
                        y: {
                            title: {
                                display: true,
                                text: 'Value'
                            }
                        }
                    }
                }
            });

            document.getElementById('dataSelect').addEventListener('change', function() {
                const columnIndex = parseInt(this.value);
                chart.data.datasets[0].data = getData(columnIndex);
                chart.data.datasets[0].label = this.options[this.selectedIndex].text;
                chart.update();
            });
        });
    </script>
</body>
</html>