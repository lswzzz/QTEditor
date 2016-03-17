#pragma once

#include "StringUtil.h"

class ByteArray {
public:
	ByteArray(bool bigEndian, unsigned int size=32);
	ByteArray(bool bigEndian, unsigned char* bytes, unsigned int size);
	~ByteArray();

	const unsigned char* getBytes() const;

	unsigned int getLength();
	void setLength(unsigned int len);

	unsigned int getPosition();
	void setPosition(unsigned int pos);

	unsigned int getBytesAvailable();

	char readChar();
	unsigned char readUnsignedChar();
	void writeChar(char value);
	void writeUnsignedChar(unsigned char value);

	short readShort();
	unsigned short readUnsignedShort();
	void writeShort(short value);
	void writeUnsignedShort(unsigned short value);

	int readInt();
	unsigned int readUnsignedInt();
	void writeInt(int value);
	void writeUnsignedInt(unsigned int value);

	float readFloat();
	void writeFloat(float value);

	void readBytes(char* bytes, unsigned int offset=0, unsigned int length=0);
	void readBytes(ByteArray* ba, unsigned int offset=0, unsigned int length=0);
	void writeBytes(char* bytes, unsigned int offset, unsigned int length);
	void writeBytes(ByteArray* ba, unsigned int offset=0, unsigned int length=0);

	std::string readString();
	void writeString(std::string str);

	bool readBool();
	void writeBool(bool b);

protected:
	unsigned char* _bytes;
	unsigned int _position;
	unsigned int _length;
	unsigned int _rawLength;
	bool _bigEndian;
	bool _isExternalData;

	void _resize(unsigned int len);
	void _checkLength(unsigned int len);
};