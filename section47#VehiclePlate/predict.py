import cv2 as cv
import numpy as np
from numpy.linalg import norm
import matplotlib.pyplot as plt
import sys, os, json, random

SZ = 20  # 训练图片长宽

# 来自opencv的sample，用于svm训练
def deskew(img):
	m = cv.moments(img)
	if abs(m['mu02']) < 1e-2:
		return img.copy()
	skew = m['mu11'] / m['mu02']
	M = np.float32([[1, skew, -0.5 * SZ * skew], [0, 1, 0]])
	img = cv.warpAffine(img, M, (SZ, SZ), flags=cv.WARP_INVERSE_MAP | cv.INTER_LINEAR)
	return img

# 来自opencv的sample，用于svm训练
def preprocess_hog(digits):
	samples = []
	for img in digits:
		gx = cv.Sobel(img, cv.CV_32F, 1, 0)
		gy = cv.Sobel(img, cv.CV_32F, 0, 1)
		mag, ang = cv.cartToPolar(gx, gy)
		bin_n = 16
		bin = np.int32(bin_n * ang / (2 * np.pi))
		bin_cells = bin[:10, :10], bin[10:, :10], bin[:10, 10:], bin[10:, 10:]
		mag_cells = mag[:10, :10], mag[10:, :10], mag[:10, 10:], mag[10:, 10:]
		hists = [np.bincount(b.ravel(), m.ravel(), bin_n) for b, m in zip(bin_cells, mag_cells)]
		hist = np.hstack(hists)

		# transform to Hellinger kernel
		eps = 1e-7
		hist /= hist.sum() + eps
		hist = np.sqrt(hist)
		hist /= norm(hist) + eps

		samples.append(hist)
	return np.float32(samples)

class SVM:
	def __init__(self, C=1, gamma=0.5):
		self.model = cv.ml.SVM_create()
		self.model.setGamma(gamma)
		self.model.setC(C)
		self.model.setKernel(cv.ml.SVM_RBF)
		self.model.setType(cv.ml.SVM_C_SVC)

	# 训练svm
	def train(self, samples, responses):
		self.model.train(samples, cv.ml.ROW_SAMPLE, responses)

	# 字符识别
	def predict(self, samples):
		r = self.model.predict(samples)
		return r[1].ravel()

	def load(self, fn):
		self.model = self.model.load(fn)

	def save(self, fn):
		self.model.save(fn)

class LPRAlg:
	DEBUG		= False
	maxLength 	= 700
	minArea 	= 2000

	imgPlatList 	= []
	colorList 		= []
	imgPlatSplitList 	= []
	charPlatList		= []
	colorPlatList		= []

	def __init__(self, imgPath = None):
		if imgPath is None:
			print("Please input correct path!")
			return None

		self.imgOri = cv.imread(imgPath)
		if self.imgOri is None:
			print("Cannot load this picture!")
			return None

		# self.cvImShow("imgOri", self.imgOri)

		# 初始化SVM模型
		self.modle = SVM(C=1, gamma=0.5)

	def cvImShow(self, arg, *args, **kwargs):
		if self.DEBUG:
			cv.imshow(arg, *args, **kwargs)

	def getNuCol(self):
		self.findVehiclePlate()
		self.splitCharacter()
		self.svmTrain()
		self.svmPredict()

		return self.charPlatList, self.colorPlatList

	def svmPredict(self):
		if not os.path.exists("svm.xml"):
			return
		# Step3: 加载和预测
		self.modle.load("svm.xml")
		for group, imgPlatSplit in enumerate(self.imgPlatSplitList):
			charList = []
			for index, imgPredict in enumerate(imgPlatSplit):
				# imgPredict = cv.imread("Nu3.jpg")
				if len(imgPredict.shape) == 3:
					imgPredict = cv.cvtColor(imgPredict, cv.COLOR_BGR2GRAY)

				w = abs(imgPredict.shape[1] - SZ) // 2

				imgPredict = cv.copyMakeBorder(imgPredict, 0, 0, w, w, cv.BORDER_CONSTANT, value=[0, 0, 0])
				imgPredict = cv.resize(imgPredict, (SZ, SZ), interpolation=cv.INTER_AREA)
				# self.cvImShow("imgPredict " + str(group) + str(index), imgPredict)
				# cv.imwrite("imgPredict_" + str(group) + str(index)+".png", imgPredict)
				imgPredict = preprocess_hog([imgPredict])
				predictCh = self.modle.predict(imgPredict)
				charList.append(chr(predictCh))
				print(chr(predictCh))
			self.charPlatList.append(charList)

			print("imgPlatSplitList " + str(group))


		# imgPredict = cv.imread("imgPredict_3.png")
		# if len(imgPredict.shape) == 3:
		# 	imgPredict = cv.cvtColor(imgPredict, cv.COLOR_BGR2GRAY)
		#
		# imgPredict = preprocess_hog([imgPredict])
		# predictCh = self.modle.predict(imgPredict)
		# print(chr(predictCh))

	def svmTrain(self):
		def cvImread(filePath=""):
			imgMat = cv.imdecode(np.fromfile(filePath, dtype=np.uint8), -1)
			return imgMat

		if os.path.exists("svm.xml"):
			# self.modle.load("svm.xml")
			return

		svmTrain = []
		svmLabel = []

		# Step1: 数据提取和打标签
		for root, dirs, files in os.walk("train\\data"):
			# print(root, dirs, files)
			if len(os.path.basename(root)) > 1:	# 轮询到最后一层
				continue
			# rootChar = os.path.basename(root)
			rootChar = ord(os.path.basename(root))
			print("--->", os.path.basename(root))

			for file in files:
				filePath = os.path.join(root, file)
				imgTemp = cvImread(filePath)
				# self.cvImShow("imgTemp", imgTemp)
				if len(imgTemp.shape) == 3:
					imgTemp = cv.cvtColor(imgTemp, cv.COLOR_BGR2GRAY)
				ret, imgTemp = cv.threshold(imgTemp, 0, 255, cv.THRESH_BINARY + cv.THRESH_OTSU)
				svmTrain.append(imgTemp)
				svmLabel.append(rootChar)

		svmTrain = list(map(deskew, svmTrain))
		svmTrain = preprocess_hog(svmTrain)
		svmLabel = np.array(svmLabel)
		print(svmTrain.shape, svmLabel.shape)

		# Step2: 训练和保存
		self.modle.train(svmTrain, svmLabel)

		print("----DONE----")
		self.modle.save("svm.xml")

	def splitCharacter(self):
		# 根据设定的阈值和图片直方图，找出波峰，用于分隔字符
		def findWaves(threshold, histogram):
			upPoint = -1  # 上升点
			isPeak = False
			if histogram[0] > threshold:
				upPoint = 0
				isPeak = True
			wakePeakList = [] # Point的List，(上升点，下降点)，表示一个波峰的宽度！
			for i, x in enumerate(histogram):
				if isPeak and x < threshold:
					if i - upPoint > 2:
						isPeak = False
						wakePeakList.append((upPoint, i))
				elif not isPeak and x >= threshold:
					isPeak = True
					upPoint = i
			if isPeak and upPoint != -1 and i - upPoint > 4:
				wakePeakList.append((upPoint, i))
			return wakePeakList

		# 根据找出的波峰，分隔图片，从而得到逐个字符图片
		def separateCard(img, waves):
			partCards = []
			for wave in waves:
				partCards.append(img[:, wave[0]:wave[1]]) # 只分割列，从wave[0]列到wave[1]列
			return partCards

		# 判断筛选出来的车牌区域
		if len(self.imgPlatList) == 0:
			print("carPlateList is None")
			return

		# 判断筛选出来的车牌颜色
		if len(self.colorList) == 0:
			print("colorList is None")
			return

		# 依次轮询车牌颜色，实际也是轮询了车牌图片区域。
		for index, color in enumerate(self.colorList):
			partCards = []
			if color in ("blue", "green", "yellow"):
				# Step1: 图片预处理
				print("[color, index]: ", color, index)
				imgPlat = self.imgPlatList[index]
				imgGary = cv.cvtColor(imgPlat, cv.COLOR_BGR2GRAY)
				self.cvImShow("split imgGary " + str(index), imgGary)

				# 绿色和黄色需要反色
				if color is "green" or color is "yellow":
					imgGary =cv.bitwise_not(imgGary)

				# 转换为二进制，方便统计
				ret, imgBin = cv.threshold(imgGary, 0, 255, cv.THRESH_BINARY + cv.THRESH_OTSU)
				self.cvImShow("split imgBin" + str(index), imgBin)

				# Step2: 做列方向的像素累加直方图，并找出波峰区间
				# 对疑似车牌区域进行逐行累加统计，目的是为了去掉上下边框
				xHistogram = np.sum(imgBin, axis=1)
				rowNum,colNum = imgBin.shape[:2]

				x = np.linspace(0, rowNum, rowNum)
				plt.figure("imgPlatList " + str(index))

				plt.subplot(1, 2, 1)
				plt.title("Row Sum")
				plt.plot(x, xHistogram)
				# plt.show()

				xMin = np.min(xHistogram)
				xAvarge = np.sum(xHistogram) / xHistogram.shape[0]
				xThreshold = (xMin + xAvarge) / 2

				wavePeakList = findWaves(xThreshold, xHistogram)
				if len(wavePeakList) == 0:
					print("Peak Number is 0")
					continue

				wave = max(wavePeakList, key=lambda x: x[1] - x[0])
				imgBin =imgBin[wave[0]:wave[1]]
				self.cvImShow("imgBin " + str(index), imgBin)

				# 对疑似车牌区域进行逐列累加，其目的是为了分割各个字符（汉字，分隔符，字母，数字）
				yHistogram = np.sum(imgBin, axis=0)
				x = np.linspace(0, colNum, colNum)
				plt.subplot(1, 2, 2)
				plt.title("Col Sum")
				plt.plot(x, yHistogram)
				if self.DEBUG:
					plt.show()

				yMin = np.min(yHistogram)
				yAverage = np.sum(yHistogram) / yHistogram.shape[0]
				yThreshold = (yMin + yAverage) / 7  # U和0要求阈值偏小，否则U和0会被分成两半

				wavePeakList = findWaves(yThreshold, yHistogram)
				print("Index[{}], Peak Number [{}]".format(index, len(wavePeakList)))

				if len(wavePeakList) <= 6:
					print("Index[{}] Peak Less" .format(index, len(wavePeakList)))
					continue

				wave = max(wavePeakList, key=lambda x: x[1] - x[0])
				maxWaveDis = wave[1] - wave[0]

				# Step3: 分离后的图片筛选
				# 判断是否是左侧车牌边缘
				if wavePeakList[0][1] - wavePeakList[0][0] < maxWaveDis / 3 and wavePeakList[0][0] == 0:
					wavePeakList.pop(0)

				print("wavePeakList:", wavePeakList)

				# 组合分离汉字
				curDis = 0
				for i, wave in enumerate(wavePeakList):
					if wave[1] - wave[0] + curDis > maxWaveDis * 0.6:   #需要满足一定的宽的
						break
					else:
						curDis += wave[1] - wave[0]
				if i > 0:
					wave = (wavePeakList[0][0], wavePeakList[i][1])
					wavePeakList = wavePeakList[i + 1:]
					wavePeakList.insert(0, wave)

				if len(wavePeakList) < 6:
					print("This is not a Vehicle Plate")
					continue

				for i, wave in enumerate(wavePeakList):
					if wave[1] - wave[0] < maxWaveDis / 3:
						wavePeakList.pop(i)

				# # 去除车牌上的分隔点
				# point = wavePeakList[2]
				# if point[1] - point[0] < maxWaveDis / 3:
				# 	imgPoint = imgBin[:, point[0]:point[1]]
				# 	if np.mean(imgPoint) < 255 / 5:
				# 		wavePeakList.pop(2)

				if len(wavePeakList) <= 6:
					print("peak less:", len(wavePeakList))
					continue

				# Step4: 根据分割区间拆分图片
				partCards = separateCard(imgBin, wavePeakList)
				self.imgPlatSplitList.append(partCards)
				self.colorPlatList.append(color)
				for i, img in enumerate(partCards):
					self.cvImShow("partCards " + str(index) + str(i), img)


	def findVehiclePlate(self):
		def accurate_place(imgHsv, limit1, limit2, color):
			rows, cols = imgHsv.shape[:2]
			left = cols
			right = 0
			top = rows
			bottom = 0

			# rowsLimit = 21
			rowsLimit = rows * 0.8 if color != "green" else rows * 0.5  # 绿色有渐变
			colsLimit = cols * 0.8 if color != "green" else cols * 0.5  # 绿色有渐变
			for row in range(rows):
				count = 0
				for col in range(cols):
					H = imgHsv.item(row, col, 0)
					S = imgHsv.item(row, col, 1)
					V = imgHsv.item(row, col, 2)
					if limit1 < H <= limit2 and 34 < S:  # and 46 < V:
						count += 1
				if count > colsLimit:
					if top > row:
						top = row
					if bottom < row:
						bottom = row
			for col in range(cols):
				count = 0
				for row in range(rows):
					H = imgHsv.item(row, col, 0)
					S = imgHsv.item(row, col, 1)
					V = imgHsv.item(row, col, 2)
					if limit1 < H <= limit2 and 34 < S:  # and 46 < V:
						count += 1
				if count > rowsLimit:
					if left > col:
						left = col
					if right < col:
						right = col

			return left, right, top, bottom

		def zoom(w, h, wMax, hMax):
			# if w <= wMax and h <= hMax:
			# 	return w, h
			widthScale = 1.0 * wMax / w
			heightScale = 1.0 * hMax / h

			scale = min(widthScale, heightScale)

			resizeWidth = int(w * scale)
			resizeHeight = int(h * scale)

			return resizeWidth, resizeHeight

		def pointLimit(point, maxWidth, maxHeight):
			if point[0] < 0:
				point[0] = 0
			if point[0] > maxWidth:
				point[0] = maxWidth
			if point[1] < 0:
				point[1] = 0
			if point[1] > maxHeight:
				point[1] = maxHeight

		if self.imgOri is None:
			print("Please load picture frist!")
			return False

		# Step1: Resize
		img = np.copy(self.imgOri)
		h, w = img.shape[:2]
		# imgWidth, imgHeight = zoom(w, h, self.maxLength, self.maxLength)
		# print("[w, h, resizeW, resizeH]: ", w, h, imgWidth, imgHeight)
		# img =cv.resize(img, (imgWidth, imgHeight), interpolation=cv.INTER_AREA)
		# 将图片剪裁为固定大小
		if w > self.maxLength:
			resizeRate = self.maxLength / w
			img = cv.resize(img, (self.maxLength, int(h * resizeRate)), interpolation=cv.INTER_AREA)
			w, h = self.maxLength, int(h*resizeRate)
		imgWidth, imgHeight = w, h
		self.cvImShow("imgResize", img)

		# Step2: Prepare to find contours
		img = cv.GaussianBlur(img, (3, 3), 0)
		imgGary = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
		self.cvImShow("imgGary", imgGary)

		kernel = np.ones((20, 20), np.uint8)
		imgOpen = cv.morphologyEx(imgGary, cv.MORPH_OPEN, kernel)
		self.cvImShow("imgOpen", imgOpen)

		imgOpenWeight = cv.addWeighted(imgGary, 1, imgOpen, -1, 0)
		self.cvImShow("imgOpenWeight", imgOpenWeight)

		ret, imgBin = cv.threshold(imgOpenWeight, 0, 255, cv.THRESH_OTSU + cv.THRESH_BINARY)
		self.cvImShow("imgBin", imgBin)

		imgEdge = cv.Canny(imgBin, 100, 200)
		self.cvImShow("imgEdge", imgEdge)

		kernel = np.ones((4, 19), np.uint8)
		imgEdge = cv.morphologyEx(imgEdge, cv.MORPH_CLOSE, kernel)
		imgEdge = cv.morphologyEx(imgEdge, cv.MORPH_OPEN, kernel)
		self.cvImShow("imgEdgeProcessed", imgEdge)

		# Step3: Find Contours
		image, contours, hierarchy = cv.findContours(imgEdge, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
		contours = [cnt for cnt in contours if cv.contourArea(cnt) > self.minArea]

		# Step4: Delete some rects
		carPlateList = []
		imgDark = np.zeros(img.shape, dtype = img.dtype)
		for index, contour in enumerate(contours):
			rect = cv.minAreaRect(contour) # [中心(x,y), (宽,高), 旋转角度]
			w, h = rect[1]
			if w < h:
				w, h = h, w
			scale = w/h
			if scale > 2 and scale < 5.5:
				# color = (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
				color = (255, 255, 255)
				carPlateList.append(rect)
				cv.drawContours(imgDark, contours, index, color, 1, 8)

				box = cv.boxPoints(rect)  # Peak Coordinate
				box = np.int0(box)
				# Draw them out
				cv.drawContours(imgDark, [box], 0, (0, 0, 255), 1)

		self.cvImShow("imgGaryContour", imgDark)
		print("Vehicle number: ", len(carPlateList))

		# Step5: Rect rectify
		imgPlats = []
		for index, carPlat in enumerate(carPlateList):
			if carPlat[2] > -1 and carPlat[2] < 1:
				angle = 1
			else:
				angle = carPlat[2]

			carPlat = (carPlat[0], (carPlat[1][0] + 5, carPlat[1][1] + 5), angle)
			box = cv.boxPoints(carPlat) 

			# Which point is Left/Right/Top/Bottom
			w, h = carPlat[1][0], carPlat[1][1]
			if w > h:
				LT = box[1]
				LB = box[0]
				RT = box[2]
				RB = box[3]
			else:
				LT = box[2]
				LB = box[1]
				RT = box[3]
				RB = box[0]

			for point in [LT, LB, RT, RB]:
				pointLimit(point, imgWidth, imgHeight)

			# Do warpAffine
			newLB = [LT[0], LB[1]]
			newRB = [RB[0], LB[1]]
			oldTriangle = np.float32([LT, LB, RB])
			newTriangle = np.float32([LT, newLB, newRB])
			warpMat = cv.getAffineTransform(oldTriangle, newTriangle)
			imgAffine = cv.warpAffine(img, warpMat, (imgWidth, imgHeight))
			self.cvImShow("imgAffine" + str(index), imgAffine)
			# print("Index: ", index)

			imgPlat = imgAffine[int(LT[1]):int(newLB[1]), int(newLB[0]):int(newRB[0])]
			imgPlats.append(imgPlat)
			self.cvImShow("imgPlat" + str(index), imgPlat)

		#Step6: Find correct place by color.
		# colorList = []
		for index, imgPlat in enumerate(imgPlats):
			green = yellow = blue = 0
			imgHsv = cv.cvtColor(imgPlat, cv.COLOR_BGR2HSV)
			rows, cols = imgHsv.shape[:2]
			imgSize = cols * rows
			color = None

			for row in range(rows):
				for col in range(cols):
					H = imgHsv.item(row, col, 0)
					S = imgHsv.item(row, col, 1)
					V = imgHsv.item(row, col, 2)

					if 11 < H <= 34 and S > 34:
						yellow += 1
					elif 35 < H <= 99 and S > 34:
						green += 1
					elif 99 < H <= 124 and S > 34:
						blue += 1

			limit1 = limit2 = 0
			if yellow * 3 >= imgSize:
				color = "yellow"
				limit1 = 11
				limit2 = 34
			elif green * 3 >= imgSize:
				color = "green"
				limit1 = 35
				limit2 = 99
			elif blue * 3 >= imgSize:
				color = "blue"
				limit1 = 100
				limit2 = 124

			print("Image Index[", index, '], Color：', color)
			print("[Blue, Green, Yellow, imgSize]: ", blue, green, yellow, imgSize)

			if color is None:
				print("Index[", index, "], Color is None!")
				continue

			# Step7: Resize vehicle img.
			left, right, top, bottom = accurate_place(imgHsv, limit1, limit2, color)
			w = right - left
			h = bottom - top

			if left == right or top == bottom:
				print("Index[", index, "]Resize vehicle img fail")
				continue

			scale = w/h
			if scale < 2 or scale > 5.5:
				print("Index[", index, "]The size is not correct!")
				continue


			needAccurate = False
			if top >= bottom:
				top = 0
				bottom = rows
				needAccurate = True
			if left >= right:
				left = 0
				right = cols
				needAccurate = True
			# imgPlat[index] = imgPlat[top:bottom, left:right] \
			# if color != "green" or top < (bottom - top) // 4 \
			# else imgPlat[top - (bottom - top) // 4:bottom, left:right]
			imgPlats[index] = imgPlat[top:bottom, left:right]
			self.cvImShow("Vehicle Image " + str(index), imgPlats[index])
			self.colorList.append(color)
			self.imgPlatList.append(imgPlats[index])
			print("---->Vehicle Image ", index)


if __name__ == '__main__':
	L = LPRAlg("2.jfif")
	L.findVehiclePlate()
	L.splitCharacter()
	# cv.waitKey(0)
	L.svmTrain()
	L.svmPredict()
	cv.waitKey(0)
