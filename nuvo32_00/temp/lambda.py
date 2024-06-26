import boto3
import json

def lambda_handler(event, context):
    endpoint = 'https://自分のエンドポイントを記載すること-ats.iot.ap-northeast-1.amazonaws.com'
    print("Received event: ")
    command_value = event.get('command')
    
    # S3クライアントの作成
    s3 = boto3.client('s3')
    iot_client = boto3.client('iot-data', endpoint_url = endpoint)  # リージョンを指定
    topic = 'nuvo32/sub'
    chunk_size = 1024 # 128 * 1024 - 1000  # チャンクサイズを128KBより少し小さく設定
    
    # S3バケット名とファイル名の指定
    bucket_name = 'nuvo32'
    if command_value == 0:
        file_key = 'mono_update_res2.bin'
    else:
        file_key = 'mono_update_res1.bin'
    print(file_key)
    
    # S3からファイルを取得
    response = s3.get_object(Bucket=bucket_name, Key=file_key)
    file_content = response['Body'].read()
    
    # ファイルコンテンツをHexエンコード
    hex_file_content = file_content.hex()
    
    for i in range(0, len(file_content), chunk_size):
        chunk = file_content[i:i + chunk_size]
        message = {
            'i': i // chunk_size
            # 'total_chunks': (len(file_content) + chunk_size - 1) // chunk_size
        }
        
        response = iot_client.publish(
            topic=topic,
            qos=1,
            payload=json.dumps(message)
        )
    
        response = iot_client.publish(
            topic=topic,
            qos=1,
            payload=chunk
        )
    
    message = {
        # 'i': i // chunk_size
        'end': (len(file_content) + chunk_size - 1) // chunk_size
    }
    
    response = iot_client.publish(
        topic=topic,
        qos=1,
        payload=json.dumps(message)
    )
    
    
    # ファイル内容をログに出力
    # print(message)
