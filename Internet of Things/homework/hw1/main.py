"""
Matt Kaufman
Internet of Things

Homework 1 - 02/01/2023
"""
from scipy import stats
import matplotlib.pyplot as plt


attendance    =  [ 7,  9,  9,  9,  9, 10, 10, 10, 10, 10, 11, 11, 12, 12, 12, 12, 13, 13, 14, 15, 15, 15, 15, 15, 15, 16, 16, 17, 17]
score_points  =  [31, 11, 22, 50, 61,  1, 15, 50, 52, 78, 43, 63, 30, 50, 65, 91, 60, 61, 38, 45, 68, 72, 83, 90, 50, 53, 78, 82, 88]


slope, intercept = stats.linregress(attendance, score_points)


def best_fit(x):
    return slope * x + intercept


line = [best_fit(x) for x in range(0, 19)]





plt.figure(figsize=(10, 6))
plt.plot(attendance, score_points, 'o', markersize=5, markeredgecolor='black', markeredgewidth=1, alpha=0.8)
plt.plot(range(0, 19), line, 'r', linewidth=1.5, alpha=0.8, label='y = %.2fx + %.2f' % (slope, intercept))
plt.plot(9, best_fit(9), 'o', markersize=7, markeredgecolor='black', markeredgewidth=1, alpha=0.8, markerfacecolor='red', label='Attendance = 9')
plt.plot([0,9], [best_fit(9),best_fit(9)], 'r--', linewidth=1, alpha=0.6)
plt.plot([9,9], [0,best_fit(9)], 'r--', linewidth=1, alpha=0.6)
plt.plot(14, best_fit(14), 'o', markersize=7, markeredgecolor='black', markeredgewidth=1, alpha=0.8, markerfacecolor='green', label='Attendance = 14')
plt.plot([0,14], [best_fit(14),best_fit(14)], 'g--', linewidth=1, alpha=0.6)
plt.plot([14,14], [0,best_fit(14)], 'g--', linewidth=1, alpha=0.6)
plt.title('Class Attendance vs Student Performance',   fontsize=18, pad=20)
plt.xlabel('Attendance', fontsize=14, fontstyle='italic', labelpad=10)
plt.ylabel('Score Points', fontsize=14, fontstyle='italic', labelpad=15)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.grid(linestyle='--', linewidth=0.5, alpha=0.3)
plt.grid(linestyle='--', linewidth=0.5, alpha=0.3, axis='y')
plt.xlim(0, 18)
plt.ylim(0, 100)
plt.legend(loc='upper left', fontsize=12)
plt.show()

