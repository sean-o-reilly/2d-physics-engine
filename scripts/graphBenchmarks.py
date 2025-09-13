# This script parses benchmark log files and generates an interactive html plot of frame times using Plotly.
# Will treat first line of each log file as the title for that data series.

import os
import re
import plotly.graph_objs as go
from plotly.offline import plot

def parse_benchmark_file(filepath):
	with open(filepath, 'r') as f:
		lines = f.readlines()
	if not lines:
		return None, [], []
	title = lines[0].strip()
	frame_times = []
	ms_pattern = re.compile(r'([\d.]+)\s*ms')
	for line in lines[1:]:
		ms_match = ms_pattern.search(line)
		if ms_match:
			frame_times.append(float(ms_match.group(1)))
	entry_numbers = list(range(1, len(frame_times) + 1))
	return title, entry_numbers, frame_times

def main():
	logs_dir = os.path.join(os.path.dirname(__file__), '..', 'logs')
	traces = []
	for root, dirs, files in os.walk(logs_dir):
		for filename in files:
			if filename.lower().startswith('benchmark'):
				filepath = os.path.join(root, filename)
				title, times, frame_times = parse_benchmark_file(filepath)
				if times and frame_times:
					trace = go.Scatter(x=times, y=frame_times, mode='lines', name=title)
					traces.append(trace)
	layout = go.Layout(title='Benchmark Frame Times', xaxis=dict(title='Frame Number'), yaxis=dict(title='Frame Time (ms)'))
	# Add target frame time as a trace for legend
	if traces:
		max_x = max(len(trace.x) for trace in traces)
		target_x = list(range(1, max_x + 1))
		target_y = [16.66] * max_x
		target_trace = go.Scatter(x=target_x, y=target_y, mode='lines', name='Target Frame Time (16.66 ms)',
								  line=dict(color='red', width=2, dash='dot'))
		traces.append(target_trace)
	fig = go.Figure(data=traces, layout=layout)
	plot(fig, filename='scripts/benchmark_plot.html', auto_open=True)

if __name__ == '__main__':
	main()
