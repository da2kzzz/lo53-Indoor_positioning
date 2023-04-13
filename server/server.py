from flask import Flask
from flask import request

from sqlalchemy import create_engine, Column, Integer, String, Float, ForeignKey
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship, sessionmaker

import time

app = Flask(__name__)

engine = create_engine('sqlite:///rssi.db')
base = declarative_base()
Session = sessionmaker(bind=engine)

class AccessPoint(base):
    __tablename__ = "accesspoint"
    id = Column(Integer, primary_key=True)
    mac_address = Column(String)

class Location(base):
    __tablename__ = "location"
    id = Column(Integer, primary_key=True)
    x = Column(Float)
    y = Column(Float)
    z = Column(Float)

class Sample(base):
    __tablename__ = "sample"
    ap_id = Column(Integer, ForeignKey("accesspoint.id"))
    source_address = Column(String, nullable=False, primary_key=True)
    timestamp = Column(Float, nullable=False, primary_key=True)
    rssi = Column(Float, nullable=False)
    ap = relationship("AccessPoint", backref="sample")

    def values(self, src, t, _rssi, _ap):
        source_address = src
        timestamp = t
        rssi = _rssi
        ap = _ap

@app.route("/rssi", methods=['GET', 'POST'])
def rssi():
		if request.method == 'POST':
		if request.args.get('ap'):
			
			param = request.args.get('ap')
			session = Session()
			ap = param.split(',')
			mac = ap[1].split('=')
			result = (session.query(AccessPoint).filter(AccessPoint.mac_address == ap[0]).all())
			sample = Sample()

			if len(result) == 0:
				accesspoint = AccessPoint(mac_address=ap[0])
				session.add(accesspoint)
				session.commit()
				sample.ap = accesspoint
			else:
				sample.ap = result[0]

			sample.source_address = mac[0]
			sample.timestamp = time.time()
			sample.rssi=mac[1]
			session.add(sample)
			session.commit()
    return "ok"