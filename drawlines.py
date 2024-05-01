#!/usr/bin/env python3
import plotly.graph_objects as go

def read_lines_from_file(filename):
    """
    Reads a file and extracts lines of coordinates in the format x1,y1,x2,y2.

    Args:
        filename (str): The path to the file containing the lines of coordinates.

    Returns:
        list of tuples: A list where each tuple contains four integers (x1, y1, x2, y2).
    """
    lines = []
    with open(filename, 'r') as file:
        for line in file:
            parts = line.strip().split(',')
            if len(parts) == 4:
                lines.append(tuple(map(int, parts)))
    return lines

def plot_lines(lines):
    """
    Plots lines on a 2D graph using Plotly, where each line is defined by coordinates (x1, y1, x2, y2).

    Args:
        lines (list of tuples): A list of tuples where each tuple contains four integers (x1, y1, x2, y2).
    """
    fig = go.Figure()
    for (x1, y1, x2, y2) in lines:
        fig.add_trace(go.Scatter(x=[x1, x2], y=[y1, y2], mode='lines+markers', line=dict(color='blue')))

    fig.update_layout(title='Lines from Coordinates',
                      xaxis=dict(range=[0, 512], autorange=False),
                      yaxis=dict(range=[0, 512], autorange=False),
                      width=600,
                      height=600)
    fig.write_html('plot.html')
    fig.show()

# Example usage:
filename = 'points.txt'  # This should be the path to your file.
lines = read_lines_from_file(filename)
plot_lines(lines)
